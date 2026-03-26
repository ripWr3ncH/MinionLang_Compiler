# MinionLang Project Plan and TODO

## Goal
Make the project fully submission-ready for Compiler Design lab rubric.

## Current Status Snapshot
- Lexer: implemented and integrated
- Parser: implemented and integrated
- Basic semantic checks: implemented
- Full demo script: passing
- Gap remaining: stronger execution proof and semantic depth

## Priority Plan

### Phase A: Must-Fix for Submission
- [x] Add execution-proof test set with expected outputs
- [x] Add translator compile-and-run verification step
- [x] Add function return-type consistency checks
- [x] Add basic scope-aware symbol table behavior for blocks

### Phase B: Quality Improvements
- [x] Reduce cascading syntax error noise
- [x] Improve error messages for first-failure clarity
- [x] Add more semantic negative tests (function mismatch, wrong returns)

### Phase C: Optional High-Marks Enhancements
- [x] Add simple intermediate representation (3-address style)
- [x] Add tiny optimization pass (constant folding)

## Concrete TODO Checklist

### 1) Execution Correctness Evidence
- [x] Create 3 runnable MinionLang test programs for output verification
- [x] Add expected output files
- [x] Add script to compare actual vs expected output
- [x] Document evidence in report

### 2) Semantic Layer Strengthening
- [x] Track scope enter/exit on braces
- [x] Prevent invalid cross-scope variable access
- [x] Verify function return statements match declared return type
- [x] Validate function calls against parameter count

### 3) Error Handling and Reporting
- [x] Keep lexical errors clear and line-based
- [x] Keep syntax errors focused (reduce follow-up noise)
- [x] Keep semantic errors actionable (name + reason)

### 4) Documentation and Demo
- [x] Update README with final command sequence
- [x] Keep TESTING_AND_DEBUGGING guide aligned with scripts
- [x] Prepare short viva checklist and demo order

## Suggested Work Order
1. Execution-proof tests and expected outputs
2. Return-type and function-call semantic checks
3. Scope handling improvement
4. Final docs + viva-ready run sequence

## Time-Limited Version (if deadline is close)
Do these first:
1. Execution-proof tests
2. Return-type checks
3. Final demo logs and documentation

You may skip for now:
- IR generation
- Optimizations

## Completion Tracker
- Total major tasks: 16
- Completed major tasks: 16
- Remaining major tasks: 0
