#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_TOKENS 256
#define MAX_STACK 256
#define MAX_NAME 128

typedef enum {
    TK_OPERAND = 0,
    TK_OPERATOR,
    TK_LPAREN,
    TK_RPAREN
} TokenType;

typedef struct {
    TokenType type;
    char text[MAX_NAME];
} Token;

static int temp_count = 0;

static int is_number(const char *s) {
    int i = 0;
    int dot_seen = 0;

    if (s == NULL || s[0] == '\0') return 0;
    if (s[0] == '+' || s[0] == '-') i++;
    if (s[i] == '\0') return 0;

    for (; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            if (dot_seen) return 0;
            dot_seen = 1;
        } else if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }

    return 1;
}

static int precedence(const char *op) {
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "mul") == 0 || strcmp(op, "div") == 0) {
        return 2;
    }
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "add") == 0 || strcmp(op, "minus") == 0) {
        return 1;
    }
    return 0;
}

static int is_operator_token(const char *t) {
    return strcmp(t, "+") == 0 || strcmp(t, "-") == 0 || strcmp(t, "*") == 0 || strcmp(t, "/") == 0 ||
           strcmp(t, "add") == 0 || strcmp(t, "minus") == 0 || strcmp(t, "mul") == 0 || strcmp(t, "div") == 0;
}

static const char *normalize_op(const char *op) {
    if (strcmp(op, "add") == 0) return "+";
    if (strcmp(op, "minus") == 0) return "-";
    if (strcmp(op, "mul") == 0) return "*";
    if (strcmp(op, "div") == 0) return "/";
    return op;
}

static void trim(char *s) {
    int start = 0;
    int end;

    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);

    end = (int)strlen(s) - 1;
    while (end >= 0 && isspace((unsigned char)s[end])) {
        s[end] = '\0';
        end--;
    }
}

static int tokenize_expression(const char *expr, Token *out_tokens, int *out_count) {
    int i = 0;
    int n = (int)strlen(expr);
    int count = 0;

    while (i < n) {
        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }

        if (count >= MAX_TOKENS) return 0;

        if (expr[i] == '(') {
            out_tokens[count].type = TK_LPAREN;
            strcpy(out_tokens[count].text, "(");
            count++;
            i++;
            continue;
        }

        if (expr[i] == ')') {
            out_tokens[count].type = TK_RPAREN;
            strcpy(out_tokens[count].text, ")");
            count++;
            i++;
            continue;
        }

        if (strchr("+-*/", expr[i]) != NULL) {
            out_tokens[count].type = TK_OPERATOR;
            out_tokens[count].text[0] = expr[i];
            out_tokens[count].text[1] = '\0';
            count++;
            i++;
            continue;
        }

        if (isalnum((unsigned char)expr[i]) || expr[i] == '_' || expr[i] == '.') {
            int j = 0;
            char buf[MAX_NAME];
            while (i < n && (isalnum((unsigned char)expr[i]) || expr[i] == '_' || expr[i] == '.')) {
                if (j < MAX_NAME - 1) buf[j++] = expr[i];
                i++;
            }
            buf[j] = '\0';

            if (is_operator_token(buf)) {
                out_tokens[count].type = TK_OPERATOR;
            } else {
                out_tokens[count].type = TK_OPERAND;
            }
            strncpy(out_tokens[count].text, buf, MAX_NAME - 1);
            out_tokens[count].text[MAX_NAME - 1] = '\0';
            count++;
            continue;
        }

        return 0;
    }

    *out_count = count;
    return 1;
}

static int infix_to_postfix(Token *in, int in_count, Token *out, int *out_count) {
    Token op_stack[MAX_STACK];
    int op_top = -1;
    int out_idx = 0;
    int i;

    for (i = 0; i < in_count; i++) {
        if (in[i].type == TK_OPERAND) {
            out[out_idx++] = in[i];
        } else if (in[i].type == TK_OPERATOR) {
            while (op_top >= 0 && op_stack[op_top].type == TK_OPERATOR &&
                   precedence(op_stack[op_top].text) >= precedence(in[i].text)) {
                out[out_idx++] = op_stack[op_top--];
            }
            op_stack[++op_top] = in[i];
        } else if (in[i].type == TK_LPAREN) {
            op_stack[++op_top] = in[i];
        } else if (in[i].type == TK_RPAREN) {
            while (op_top >= 0 && op_stack[op_top].type != TK_LPAREN) {
                out[out_idx++] = op_stack[op_top--];
            }
            if (op_top < 0) return 0;
            op_top--;
        }
    }

    while (op_top >= 0) {
        if (op_stack[op_top].type == TK_LPAREN) return 0;
        out[out_idx++] = op_stack[op_top--];
    }

    *out_count = out_idx;
    return 1;
}

static int fold_binary(const char *lhs, const char *rhs, const char *op, char *out) {
    double a;
    double b;
    double r;
    const char *norm = normalize_op(op);

    if (!is_number(lhs) || !is_number(rhs)) return 0;

    a = atof(lhs);
    b = atof(rhs);

    if (strcmp(norm, "+") == 0) r = a + b;
    else if (strcmp(norm, "-") == 0) r = a - b;
    else if (strcmp(norm, "*") == 0) r = a * b;
    else if (strcmp(norm, "/") == 0) {
        if (b == 0.0) return 0;
        r = a / b;
    } else return 0;

    if ((long long)r == r) {
        sprintf(out, "%lld", (long long)r);
    } else {
        sprintf(out, "%.6g", r);
    }
    return 1;
}

static int emit_tac_for_expression(const char *lhs, const char *expr, FILE *out) {
    Token infix[MAX_TOKENS];
    Token postfix[MAX_TOKENS];
    char *value_stack[MAX_STACK];
    int vtop = -1;
    int infix_count = 0;
    int postfix_count = 0;
    int i;

    if (!tokenize_expression(expr, infix, &infix_count)) return 0;
    if (!infix_to_postfix(infix, infix_count, postfix, &postfix_count)) return 0;

    for (i = 0; i < postfix_count; i++) {
        if (postfix[i].type == TK_OPERAND) {
            value_stack[++vtop] = _strdup(postfix[i].text);
            if (value_stack[vtop] == NULL) return 0;
        } else if (postfix[i].type == TK_OPERATOR) {
            char folded[MAX_NAME];
            char temp_name[MAX_NAME];
            char *right;
            char *left;
            const char *norm;

            if (vtop < 1) return 0;
            right = value_stack[vtop--];
            left = value_stack[vtop--];
            norm = normalize_op(postfix[i].text);

            if (fold_binary(left, right, postfix[i].text, folded)) {
                value_stack[++vtop] = _strdup(folded);
            } else {
                sprintf(temp_name, "t%d", ++temp_count);
                fprintf(out, "%s = %s %s %s\n", temp_name, left, norm, right);
                value_stack[++vtop] = _strdup(temp_name);
            }

            free(left);
            free(right);
            if (value_stack[vtop] == NULL) return 0;
        }
    }

    if (vtop != 0) return 0;

    fprintf(out, "%s = %s\n", lhs, value_stack[vtop]);
    free(value_stack[vtop]);
    return 1;
}

static int process_line(char *line, FILE *out) {
    char *comment_pos;
    char *assign_pos;
    char lhs[MAX_NAME];
    char expr[MAX_LINE];
    int i;

    comment_pos = strstr(line, "$$");
    if (comment_pos) *comment_pos = '\0';

    trim(line);
    if (line[0] == '\0') return 1;

    if (strncmp(line, "#bringy", 7) == 0 || strncmp(line, "#setty", 6) == 0) return 1;
    if (strchr(line, '{') || strchr(line, '}')) return 1;
    if (strncmp(line, "papoy", 5) == 0 || strncmp(line, "takey", 5) == 0) return 1;

    assign_pos = strstr(line, ":=");
    if (assign_pos == NULL) return 1;

    strncpy(expr, assign_pos + 2, sizeof(expr) - 1);
    expr[sizeof(expr) - 1] = '\0';
    trim(expr);

    if (expr[strlen(expr) - 1] == ';') expr[strlen(expr) - 1] = '\0';
    trim(expr);

    lhs[0] = '\0';
    for (i = (int)(assign_pos - line) - 1; i >= 0; i--) {
        if (isalnum((unsigned char)line[i]) || line[i] == '_') {
            int end = i;
            int start = i;
            while (start >= 0 && (isalnum((unsigned char)line[start]) || line[start] == '_')) start--;
            start++;
            strncpy(lhs, line + start, end - start + 1);
            lhs[end - start + 1] = '\0';
            break;
        }
    }

    if (lhs[0] == '\0') return 1;

    fprintf(out, "# %s := %s\n", lhs, expr);
    if (!emit_tac_for_expression(lhs, expr, out)) {
        fprintf(out, "# skipped: unsupported expression\n");
    }
    fprintf(out, "\n");
    return 1;
}

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;
    char line[MAX_LINE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.minion> [output.ir]\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    if (!in) {
        fprintf(stderr, "Error: cannot open input file %s\n", argv[1]);
        return 1;
    }

    if (argc >= 3) {
        out = fopen(argv[2], "w");
        if (!out) {
            fclose(in);
            fprintf(stderr, "Error: cannot open output file %s\n", argv[2]);
            return 1;
        }
    } else {
        out = stdout;
    }

    fprintf(out, "# MinionLang Simple IR (3-address code)\n");
    fprintf(out, "# Includes tiny constant folding\n\n");

    while (fgets(line, sizeof(line), in) != NULL) {
        process_line(line, out);
    }

    fclose(in);
    if (out != stdout) fclose(out);

    return 0;
}
