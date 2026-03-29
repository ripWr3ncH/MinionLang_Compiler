SHELL := cmd
.SHELLFLAGS := /C

FLEX := GnuWin32\bin\flex.exe
BISON := GnuWin32\bin\bison.exe
GCC := gcc

.PHONY: help all lexer parser translator ir lexer-test syntax-tests semantic-tests ir-demo exec-proof full-demo clean

help:
	@echo MinionLang Makefile targets:
	@echo   make lexer          - build lexer executable
	@echo   make parser         - build parser executable
	@echo   make translator     - build MinionLang to C translator
	@echo   make ir             - build IR generator
	@echo   make all            - build lexer, parser, translator, IR
	@echo   make lexer-test     - run lexer sample test
	@echo   make syntax-tests   - run parser syntax tests
	@echo   make semantic-tests - run semantic error tests (expected failures)
	@echo   make ir-demo        - generate IR demo output
	@echo   make exec-proof     - run execution proof suite
	@echo   make full-demo      - run complete project demo suite
	@echo   make clean          - remove generated binaries and outputs

all: lexer parser translator ir

lexer:
	$(FLEX) src\minionlang.l
	$(GCC) -o minionlexer.exe lex.yy.c

parser:
	set PATH=GnuWin32\bin;%PATH% && $(BISON) -d -v src\minionlang.y -o src\minionlang.tab.c
	$(FLEX) src\minionlang_parser.l
	$(GCC) -o minionparser.exe src\minionlang.tab.c src\lex.yy.c -I src

translator:
	$(FLEX) src\minionlang_to_c.l
	$(GCC) -o minion2c.exe lex.yy.c

ir:
	$(GCC) -o minionir.exe src\minionlang_ir.c

lexer-test: lexer
	.\minionlexer.exe tests\sample.minion tests\sample.tokens

syntax-tests: parser
	.\minionparser.exe tests\valid_syntax.minion
	-.\minionparser.exe tests\syntax_error1.minion
	-.\minionparser.exe tests\syntax_error2.minion
	-.\minionparser.exe tests\syntax_error3.minion

semantic-tests: parser
	-.\minionparser.exe tests\semantic_error1_undeclared.minion
	-.\minionparser.exe tests\semantic_error2_redeclaration.minion
	-.\minionparser.exe tests\semantic_error3_type_mismatch.minion
	-.\minionparser.exe tests\semantic_error4_non_array_index.minion
	-.\minionparser.exe tests\semantic_error5_scope_out.minion
	-.\minionparser.exe tests\semantic_error6_void_return_value.minion
	-.\minionparser.exe tests\semantic_error7_nonvoid_missing_return.minion
	-.\minionparser.exe tests\semantic_error8_func_arg_count.minion
	-.\minionparser.exe tests\semantic_error9_func_arg_type.minion
	-.\minionparser.exe tests\semantic_error10_undefined_function.minion
	-.\minionparser.exe tests\semantic_error11_return_type_mismatch.minion

ir-demo: ir
	.\minionir.exe tests\ir_demo.minion tests\ir_demo.ir

exec-proof: translator
	.\run_execution_proof_tests.bat

full-demo:
	.\run_full_demo.bat

clean:
	-del /Q /F minionlexer.exe 2>nul
	-del /Q /F minionparser.exe 2>nul
	-del /Q /F minion2c.exe 2>nul
	-del /Q /F minionir.exe 2>nul
	-del /Q /F lex.yy.c 2>nul
	-del /Q /F src\lex.yy.c 2>nul
	-del /Q /F src\minionlang.tab.c 2>nul
	-del /Q /F src\minionlang.tab.h 2>nul
	-del /Q /F src\minionlang.output 2>nul