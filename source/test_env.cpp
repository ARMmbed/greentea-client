/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <cstdio>
#include <string.h>
#include "greentea-client/test_env.h"

// Auxilary macros
#define NL "\n"

// Generic test suite transport protocol keys
const char* GREENTEA_TEST_ENV_END = "end";
const char* GREENTEA_TEST_ENV_EXIT = "__exit";
const char* GREENTEA_TEST_ENV_SYNC = "__sync";
const char* GREENTEA_TEST_ENV_TIMEOUT = "__timeout";
const char* GREENTEA_TEST_ENV_HOST_TEST_NAME = "__host_test_name";
// Test suite success code strings
const char* GREENTEA_TEST_ENV_SUCCESS = "success";
const char* GREENTEA_TEST_ENV_FAILURE = "failure";
// Test case transport protocol start/finish keys
const char* GREENTEA_TEST_ENV_TESTCASE_COUNT = "__testcase_count";
const char* GREENTEA_TEST_ENV_TESTCASE_START = "__testcase_start";
const char* GREENTEA_TEST_ENV_TESTCASE_FINISH = "__testcase_finish";
const char* GREENTEA_TEST_ENV_TESTCASE_SUMMARY = "__testcase_summary";
// Code Coverage (LCOV)  transport protocol keys
const char* GREENTEA_TEST_ENV_LCOV_START = "__coverage_start";

// Auxilary functions
static void greentea_notify_timeout(const int);
static void greentea_notify_hosttest(const char *);
static void greentea_notify_completion(const int);


/** \brief Handshake with host and send setup data (timeout and host test name)
 *  \details This function will send preamble to master.
 *           After host test name is received master will invoke host test script
 *           and add hos test's callback handlers to main event loop
 *           This function is blocking.
 */
void GREENTEA_SETUP(const int timeout, const char *host_test_name) {
    // Key-value protocol handshake function. Waits for {{__sync;...}} message
    // Sync preamble: "{{__sync;0dad4a9d-59a3-4aec-810d-d5fb09d852c1}}"
    // Example value of sync_uuid == "0dad4a9d-59a3-4aec-810d-d5fb09d852c1"
	char _key[8] = {0};
	char _value[48] = {0};
	while (1) {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        if (strcmp(_key, GREENTEA_TEST_ENV_SYNC) == 0) {
            // Found correct __sunc message
            greentea_send_kv(_key, _value);
            break;
        }
    }

    greentea_notify_timeout(timeout);
    greentea_notify_hosttest(host_test_name);
}

/** \brief Notify host (__exit message) side that test suite execution was complete
 *  \result Test suite result
 *  \details If __exit is not received by host side we will assume TIMEOUT
 */
void GREENTEA_TESTSUITE_RESULT(const int result) {
    greentea_notify_completion(result);
}

/**
 *  Test Case support
 */

/** \brief Notify host side that test case started
 *  \details test_case_name Test case name
 */
void GREENTEA_TESTCASE_START(const char *test_case_name) {
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_START, test_case_name);
}

/** \brief Notify host side that test case finished
 *  \details test_case_name Test case name
 *  \details result Test case result (0 -OK, non zero...)
 */
void GREENTEA_TESTCASE_FINISH(const char *test_case_name, const size_t passes, const size_t failed) {
    greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_FINISH, test_case_name, passes, failed);
}

/**
 *  #################################################
 *  Auxilary functions and key-value protocol support
 *  #################################################
 */


/**
 *  ############
 *  LCOV support
 *  ############
 */
#ifdef YOTTA_CFG_DEBUG_OPTIONS_COVERAGE
extern "C" void __gcov_flush(void);
extern bool coverage_report = false;

/** \brief Send code coverage (LCOV) notification to master
  * \param notify_coverage_start() PAYLOAD notify_coverage_end()
  *
 */
void greentea_notify_coverage_start(const char *path) {
    printf("{{%s;%s;", GREENTEA_TEST_ENV_LCOV_START, path);
}

/** \brief Sufix for code coverage messgae to master
 */
void greentea_notify_coverage_end() {
    printf("}}" NL);
}

#endif

/**
 *  ##########################
 *  Key-value protocol support
 *  ##########################
 */

/** \brief Send key-value (string;string) message to master
  * \param key
  * \param value String value
  *
  */
void greentea_send_kv(const char *key, const char *val) {
    if (key && val) {
        printf("{{%s;%s}}" NL, key, val);
    }
}

/** \brief Send key-value (string;integer) message to master
  * \param key Message key
  * \param value Message payload, integer value
  *
  */
void greentea_send_kv(const char *key, const int val) {
    if (key) {
        printf("{{%s;%d}}" NL, key, val);
    }
}

/** \brief Send key-value with packed success code (string;string;integer) message to master
  * \param key Message key
  * \param value Message payload, integer value
  * \param result Send additional integer formatted data
  *
  */
void greentea_send_kv(const char *key, const char *val, const int result) {
    if (key) {
        printf("{{%s;%s;%d}}" NL, key, val, result);
    }
}

/** \brief Send key-value with packed success code (string;string;integer) message to master
  * \param key Message key
  * \param value Message payload, integer value
  * \param passes Send additional integer formatted data
  * \param failures Send additional integer formatted data
  *
  */
void greentea_send_kv(const char *key, const char *val, const int passes, const int failures) {
    if (key) {
        printf("{{%s;%s;%d;%d}}" NL, key, val, passes, failures);
    }
}

/** \brief Send key-value with packed success code (string;string;integer) message to master
  * \param key Message key
  * \param value Message payload, integer value
  * \param passes Send additional integer formatted data
  * \param failures Send additional integer formatted data
  *
  */
void greentea_send_kv(const char *key, const int passes, const int failures) {
    if (key) {
        printf("{{%s;%d;%d}}" NL, key, passes, failures);
    }
}

/** \brief Send message with timeout to master
  * \param timeout Test suite timeout in seconds
  *
  */
static void greentea_notify_timeout(const int timeout) {
    greentea_send_kv(GREENTEA_TEST_ENV_TIMEOUT, timeout);
}

/** \brief Send host test name to master
  * \param host_test_name Host test name, host test will be loaded by mbedhtrun
  *
  */
static void greentea_notify_hosttest(const char *host_test_name) {
    greentea_send_kv(GREENTEA_TEST_ENV_HOST_TEST_NAME, host_test_name);
}

/** \brief Send to master information that test suite finished its execution
  * \param result TEst suite result from DUT
  *
  */
static void greentea_notify_completion(const int result) {
    const char *val = result ? GREENTEA_TEST_ENV_SUCCESS : GREENTEA_TEST_ENV_FAILURE;
#ifdef YOTTA_CFG_DEBUG_OPTIONS_COVERAGE
    coverage_report = true;
    __gcov_flush();
    coverage_report = false;
#endif
    greentea_send_kv(GREENTEA_TEST_ENV_END, val);
    greentea_send_kv(GREENTEA_TEST_ENV_EXIT, 0);
}

/**
 *  Parse engine for KV values which replaces scanf
 *  Example usage:
 *
 *  char key[10];
 *  char value[48];
 *
 *  greentea_parse_kv(key, value, 10, 48);
 *  greentea_parse_kv(key, value, 10, 48);
 *
 */


static int gettok(char *, const int);
static int getNextToken(char *, const int);
static int HandleKV(char *,  char *,  const int,  const int);
static int isstring(int);
static int _get_char();

static int CurTok = 0;

// Token defined by KiVi parser
enum Token {
    tok_eof = -1,
    tok_open = -2,          // "{{"
    tok_close = -3,         // "}}"
    tok_semicolon = -4,     // ;
    tok_string = -5         // [a-zA-Z0-9_- ]+
};

// Closure for default "get character" function
static int _get_char() {
    return getchar();
}

/** \brief parse input string for key-value pairs: {{key;value}}
  * \param out_key Ouput data with key
  * \param out_value Ouput data with value
  * \param out_key_size out_key total size
  * \param out_value_size out_value total data
  *
  * \detail This function should replace scanf used to
            check for incoming messages from master. All data
            parsed and rejected is discarded.
  *
  * success != 0 when key-value pair was found
  * success == 0 when end of the stream was found
  *
  */
int greentea_parse_kv(char *out_key,
                      char *out_value,
                      const int out_key_size,
                      const int out_value_size) {
    getNextToken(0, 0);
    while (1) {
        switch (CurTok) {
        case tok_eof:
            return 0;

        case tok_open:
            if (HandleKV(out_key, out_value, out_key_size, out_value_size)) {
                // We've found {{ KEY ; VALUE }} expression
                return 1;
            }
            break;

        default:
            // Load next token and pray...
            getNextToken(0, 0);
            break;
        }
    }
    return 0;
}

static int getNextToken(char *str, const int str_size) {
    return CurTok = gettok(str, str_size);
}

//tokenizer auxilary function, subset of punctuation characters
static int ispunctuation(int c) {
    static const char punctuation[] = "_-!@#$%^&*()=+:<>,./?\\\"'";  // No ";{}"
    for (size_t i=0; i< sizeof(punctuation); ++i) {
        if (c == punctuation[i]) {
            return 1;
        }
    }
    return 0;
}

// KEY, VALUE tokenizer auxiliary function
static int isstring(int c) {
    return (isalpha(c) ||
            isdigit(c) ||
            isspace(c) ||
            ispunctuation(c));
}

static int gettok(char *out_str, const int str_size) {
    static int LastChar = '!';
    static int str_idx = 0;

    // whitespace ::=
    while (isspace(LastChar)) {
        LastChar = _get_char();
    }

    // string ::= [a-zA-Z0-9_-!@#$%^&*()]+
    if (isstring(LastChar)) {
        str_idx = 0;
        if (out_str && str_idx < str_size - 1) {
            out_str[str_idx++] = LastChar;
        }

        while (isstring((LastChar = _get_char())))
            if (out_str && str_idx < str_size - 1) {
                out_str[str_idx++] = LastChar;
            }
        if (out_str && str_idx < str_size) {
            out_str[str_idx] = '\0';
        }

        return tok_string;
    }

    // semicolon ::= ';'
    if (LastChar == ';') {
        LastChar = _get_char();
        return tok_semicolon;
    }

    // open ::= '{{'
    if (LastChar == '{') {
        LastChar = _get_char();
        if (LastChar == '{') {
            LastChar = _get_char();
            return tok_open;
        }
    }

    // close ::= '}'
	if (LastChar == '}') {
		LastChar = _get_char();
		if (LastChar == '}') {
			//LastChar = _get_char();
			return tok_close;
		}
	}

    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = _get_char();
    return ThisChar;
}

// KiVi parser, searches for <open> <string> <semicolon> <string> <close>
static int HandleKV(char *out_key,
                    char *out_value,
                    const int out_key_size,
                    const int out_value_size) {
    // We already started with <open>
    if (getNextToken(out_key, out_key_size) == tok_string) {
        if (getNextToken(0, 0) == tok_semicolon) {
            if (getNextToken(out_value, out_value_size) == tok_string) {
                if (getNextToken(0, 0) == tok_close) {
                    // <open> <string> <semicolon> <string> <close>
                    // Found "{{KEY;VALUE}}" expression
                    return 1;
                }
            }
        }
    }
    getNextToken(0, 0);
    return 0;
}
