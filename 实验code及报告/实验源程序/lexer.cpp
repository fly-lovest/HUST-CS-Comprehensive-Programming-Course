#include"lexer.h"
int line_num = 1;
char token_text[100];
keyword n[IDENT] = {
	{"auto",AUTO},{"break",BREAK},{"case",CASE},{"char",CHAR},{"const",CONST},{"continue",CONTINUE},{"default",DEFAULT},{"do",DO},
	{"double",DOUBLE},{"else",ELSE},{"enum",ENUM},{"extern",EXTERN},{"float",FLOAT},{"for",FOR},{"goto",GOTO},{"if",IF},
	{"int",INT},{"long",LONG},{"register",REGISTER},{"return",RETURN},{"short",SHORT},{"signed",SIGNED},{"sizeof",SIZEOF},{"static",STATIC},
	{"struct",STRUCT},{"switch",SWITCH},{"typedef",TYPEDEF},{"union",UNION},{"unsigned",UNSIGNED},{"void",VOID},{"volatile",VOLATILE},{"while",WHILE},
	{"include",INCLUDE},{"define",DEFINE}
};

int gettoken(FILE* fp) {
	char c; //用于暂存读取的单个字符
	int i=0; //用于做token_text的存储
	int j=0; //用于与关键字做比对
	
	while (( c = fgetc(fp)) == ' ' || c == '\n'|| c == '\t'||c==EOF)  //跳过空白符和制表符
	{
		if (c == '\n') line_num++;	//读取换行符时计数
		if (c == EOF) return EOF;
	}
	if (c == EOF) return EOF;
	//判断标识符或者关键字
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		do {
			token_text[i++] = c;
			c = fgetc(fp);
		} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')||c=='.');
		token_text[i] = '\0';
		ungetc(c, fp);

		for (; j < IDENT; j++) {
			if (!strcmp(token_text, n[j].key)) return n[j].enum_key;
		}
		return IDENT;
	}

	//判断标识符
	if (c == '_') { 
		do {
			token_text[i++] = c;
			c = fgetc(fp);
		} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')||c=='.');
		token_text[i] = '\0';
		ungetc(c, fp);
		return IDENT;
	}

	//判断数字（整型或浮点型）
	if (c > '0' && c <= '9') {  //首非0
		token_text[i++] = c;
		c = fgetc(fp);
		if (c >= '0' && c <= '9') {
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c >= '0' && c <= '9'); 

			if (c == 'u' || c == 'U') {
stationA:  //u/U后后缀
				token_text[i++] = c;
				c = fgetc(fp);
				if (c == 'l' || c == 'L') {
					token_text[i++] = c;
					token_text[i] = '\0';
					return UNSIGNED_LONG_CONST;
				}
				else {
					ungetc(c, fp);
					token_text[i] = '\0';
					return UNSIGNED_CONST;
				}
			}
			else if (c == 'l' || c == 'L') {
stationB: //l/L后缀(int)
				token_text[i++] = c;
				token_text[i] = '\0';
				return LONG_CONST;
			}
			else if (c == '.') {
				goto station1;
			}
			else if (c == 'e' || c == 'E') {
				goto stationE;
			}
			else {
				ungetc(c, fp);
				token_text[i] = '\0';
				return INT_CONST;
			}

		}
		else if (c == '.') {
station1://情况1（小数点）*可能没数字情况
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c >= '0' && c <= '9');
			if(c=='f'||c=='F'){
stationC: //f/F后缀
				token_text[i++] = c;
				token_text[i] = '\0';
				return FLOAT_CONST;
			}
			else if (c == 'l' || c == 'L') {
stationD: //l/L后缀（double）
				token_text[i++] = c;
				token_text[i] = '\0';
				return LONG_DOUBLE_CONST;
			}
			else if (c == 'e' || c == 'E') {
stationE:  //e/E的情况
				token_text[i++] = c;
				c = fgetc(fp);
				if (c >= '0' && c <= '9') {
					do {
						token_text[i++] = c;
						c = fgetc(fp);
					} while (c >= '0' && c <= '9');
					if (c == 'f' || c == 'F') {
						goto stationC;
					}
					else if (c == 'l' || c == 'L') {
						goto stationD;
					}
					else {
						ungetc(c, fp);
						token_text[i] = '\0';
						return DOUBLE_CONST;
					}
				}	
				else goto stationERROR;
			}
			else {
				ungetc(c, fp);
				token_text[i] = '\0';
				return DOUBLE_CONST;
			}

		}
		else if (c == 'u' || c == 'U') {
			goto stationA;
		}
		else if (c == 'l' || c == 'L') {
			goto stationB;
		}
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
stationERROR:
			ungetc(c, fp);
			token_text[i] = '\0';
			return ERROR_TOKEN;
		}
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return INT_CONST;
		}
	}


	if (c == '0') {  //首为0
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == 'x' || c == 'X') {  //判断十六进制数
			token_text[i++] = c;
			c = fgetc(fp);
			if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
				do {
					token_text[i++] = c;
					c = fgetc(fp);
				} while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
				if (c == 'u' || c == 'U') goto stationA;
				else if (c == 'l' || c == 'L') goto stationB;
				else if ((c >= 'g' && c <= 'z' && c != 'u' && c != 'l') || (c >= 'G' && c <= 'Z' && c != 'U' && c != 'L')) goto stationERROR;
				else {
					ungetc(c, fp);
					token_text[i] = '\0';
					return INT_CONST;
				}
			}
			else goto stationERROR;
		}
		else if (c >= '0' && c <= '7') { //判断八进制数
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c >= '0' && c <= '7');
			if (c == 'u' || c == 'U') goto stationA;
			else if (c == 'l' || c == 'L') goto stationB;
			else if ((c >= 'a' && c <= 'z' && c != 'u' && c != 'l') || (c >= 'A' && c <= 'Z' && c != 'U' && c != 'L') || c == '_' || (c >= '8' && c <= '9')) goto stationERROR;
			else {
				ungetc(c, fp);
				token_text[i] = '\0';
				return INT_CONST;
			}
		}
		else if (c == '.') goto station1;
		else if (c == 'u' || c == 'U') goto stationA;
		else if (c == 'l' || c == 'L') goto stationB;
		else if ((c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z' ) || c == '_' || (c >= '8' && c <= '9')) goto stationERROR;
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return INT_CONST;
		}
		
	}

	if (c == '.') {  //出现小数点的情况*后面必须出现数字
		token_text[i++] = c;
		c = fgetc(fp);
		if (c >= '0' && c <= '9') {
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c >= '0' && c <= '9');

			if (c == 'f' || c == 'F')
			{
				goto stationC;
			}
			else if (c == 'l' || c == 'L')
			{
				goto stationD;
			}
			else if (c == 'e' || c == 'E') {
				goto stationE;
			}
			else {
				ungetc(c, fp);
				token_text[i] = '\0';
				return DOUBLE_CONST;
			}
		}
		else {
			goto stationERROR;
		}
	}

	switch (c) {
	case '=':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '=') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return EQUAL;
		}
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return EQUAL_TO;
		}
	case'{':
		token_text[i++] = c;
		token_text[i] = '\0';
		return LL;

	case'}':
		token_text[i++] = c;
		token_text[i] = '\0';
		return RL;

	case'[':
		token_text[i++] = c;
		token_text[i] = '\0';
		return LM;

	case']':
		token_text[i++] = c;
		token_text[i] = '\0';
		return RM;

	case'(':
		token_text[i++] = c;
		token_text[i] = '\0';
		return LS;

	case')':
		token_text[i++] = c;
		token_text[i] = '\0';
		return RS;

	case';':
		token_text[i++] = c;
		token_text[i] = '\0';
		return SEMI;

	case',':
		token_text[i++] = c;
		token_text[i] = '\0';
		return COMMA;

	case'#':
		token_text[i++] = c;
		token_text[i] = '\0';
		return POUND;

	case'>':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '=') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return MORE_EQUAL;
		}
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return MORE;
		}

	case'<':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '=') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return LESS_EQUAL;
		}
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return LESS;
		}

	case'!':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '=') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return UNEQUAL;
		}
		else {
			goto stationERROR;
		}

	case'&':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '&') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return AND;
		}
		else {
			goto stationERROR;
		}

	case'|':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '|') {
			token_text[i++] = c;
			token_text[i] = '\0';
			return OR;
		}
		else {
			goto stationERROR;
		}

	case'+':
		token_text[i++] = c;
		token_text[i] = '\0';
		return PLUS;

	case'-':
		token_text[i++] = c;
		token_text[i] = '\0';
		return MINUS;

	case'*':
		token_text[i++] = c;
		token_text[i] = '\0';
		return MULTIPLY;

	case'%':
		token_text[i++] = c;
		token_text[i] = '\0';
		return MOD;

	case'/':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '/') {   //判断行注释
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c != '\n' && c != EOF);
			if (c == '\n'|| c == EOF) {
				ungetc(c, fp);
				token_text[i] = '\0';
				return LINENOTE;
			}
		}
		else if (c == '*') {   //判断块注释
station2://临时判断是否是*/的情况
			do {
				token_text[i++] = c;
				c = fgetc(fp);
			} while (c != '\n' && c != EOF&&c!='*');
			if (c == '*') {
				token_text[i++] = c;
				c = fgetc(fp);
				if (c == '/') {
					token_text[i++] = c;
					token_text[i] = '\0';
					return BLOCKNOTE;
				}
				else goto station2;
			}
			else if (c == '\n') {
				line_num++;
				goto station2;
			}
			else if (c == EOF) {
				goto stationERROR;
			}
		}
		else {
			ungetc(c, fp);
			token_text[i] = '\0';
			return DIVIDE;
		}

	case'\'':
		token_text[i++] = c;
		c = fgetc(fp);
		if (c == '\\') {   //判断‘\x’的情况
			token_text[i++] = c;
			c = fgetc(fp);
			if (c >= '0' && c <= '9') {  //判断'\数字'的情况
				do {
					token_text[i++] = c;
					c = fgetc(fp);
				} while (c >= '0' && c <= '9');
				if (c == '\'') {  
					token_text[i++] = c;
					token_text[i] = '\0';
					return CHAR_CONST;
				}
				else goto stationERROR;
			}
			else if (c == '\'') {  //判断'\''的情况
				token_text[i++] = c;
				c = fgetc(fp);
				if (c == '\'') {
					token_text[i++] = c;
					token_text[i] = '\0';
					return CHAR_CONST;
				}
				else goto stationERROR;
			}
			else {  //判断'\其他'的情况
				token_text[i++] = c;
				c = fgetc(fp);
				if (c == '\'') {
					token_text[i++] = c;
					token_text[i] = '\0';
					return CHAR_CONST;
				}
				else { 
					do {
						token_text[i++] = c;
						c = fgetc(fp);
					} while (c != '\'' && c != '\n'&& c!=EOF); //添加智能识别功能，防止无限报错【亮点】
					if (c == '\'') {
						return ERROR_TOKEN;
					}
					else goto stationERROR; 
				}
			}
		}
		else if (c == '\'') {  //判断'''的情况
			c = fgetc(fp);
			if (c == '\'') return ERROR_TOKEN;
			else goto stationERROR;
		}
		else {  //判断‘x’的情况，x不能为'和\在上面讨论过
			token_text[i++] = c;
			c = fgetc(fp);
			if (c == '\'') {
				token_text[i++] = c;
				token_text[i] = '\0';
				return CHAR_CONST;
			}
			else {
				do {
					token_text[i++] = c;
					c = fgetc(fp);
				} while (c != '\'' && c != '\n' && c != EOF); //添加智能识别功能，防止无限报错【亮点】
				if (c == '\'') {
					return ERROR_TOKEN;
				}
				else goto stationERROR;
			}
		}

	case'"':
		token_text[i++] = c;
		while ((c = fgetc(fp)) != '"') {
			token_text[i++] = c;
			if (c == '\\') {
				c = fgetc(fp);
				if (c == '"') {  //识别字符串最后一个字符为'\\'的错误情况【亮点】
					do {
						token_text[i++] = c;
						c = fgetc(fp);
					} while ((c != '"' )&& (c != '\n' )&& (c != EOF));
					if (c == '"') { token_text[i++] = c; token_text[i] == '\0'; return STRING_CONST; }
					else goto stationERROR;
				}
				else if (c == '\n') { line_num++; token_text[i++] = c; }
				else token_text[i++] = c;
			}
			else if (c == '\n') { //判断直接换行情况【亮点】
				line_num++;
				do {
					token_text[i++] = c;
					c = fgetc(fp);
				} while (c != '"' && c != '\n' && c != EOF);
				if (c == '"') { return ERROR_TOKEN; }
				else goto stationERROR;
			}
			else if (c == EOF) goto stationERROR;
		}
		token_text[i++] = c;
		token_text[i] = '\0';
		return STRING_CONST;

	default:
		if (c == EOF) return EOF;
		else return ERROR_TOKEN;

	}//switch结束


}