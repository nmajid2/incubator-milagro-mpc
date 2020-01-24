/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/*
 * Test utilities definitions.
 */

#include <string.h>
#include "test.h"

/* TV reading Utilities */

void read_OCTET(FILE *fp, octet *OCT, char *string)
{
    int len = strlen(string);
    char buff[len];
    memcpy(buff, string, len);
    char *end = strchr(buff, ',');
    if (end == NULL)
    {
        fclose(fp);

        printf("ERROR unexpected test vector %s\n", string);
        exit(EXIT_FAILURE);
    }
    end[0] = '\0';
    OCT_fromHex(OCT, buff);
}

void read_FF_2048(FILE *fp, BIG_1024_58 *x, char *string, int n)
{
    int len = strlen(string);
    char oct[len / 2];
    octet OCT = {0, len / 2, oct};

    read_OCTET(fp, &OCT, string);
    FF_2048_fromOctet(x, &OCT, n);
}

void scan_int(int *v, char *line, const char *prefix)
{
    if (!strncmp(line, prefix, strlen(prefix)))
    {
        line+=strlen(prefix);
        sscanf(line, "%d\n", v);
    }
}

void scan_OCTET(FILE *fp, octet *OCT, char *line, const char *prefix)
{
    if (!strncmp(line, prefix, strlen(prefix)))
    {
        line+=strlen(prefix);
        read_OCTET(fp, OCT, line);
    }
}

void scan_FF_2048(FILE *fp, BIG_1024_58 *x, char *line, const char *prefix, int n)
{
    if (!strncmp(line, prefix, strlen(prefix)))
    {
        line+=strlen(prefix);
        read_FF_2048(fp, x, line, n);
    }
}

/* Assertion utilities */

void compare_OCT(FILE* fp, int testNo, char *name, octet *X, octet *Y)
{
    if (!OCT_comp(X, Y))
    {
        if (fp != NULL)
        {
            fclose(fp);
        }

        printf("FAILURE %s. Test %d\n", name, testNo);
        exit(EXIT_FAILURE);
    }
}

void compare_FF_2048(FILE* fp, int testNo, char* name, BIG_1024_58 *x, BIG_1024_58 *y, int n)
{
    if(FF_2048_comp(x, y, n))
    {
        if (fp != NULL)
        {
            fclose(fp);
        }

        fprintf(stderr, "FAILURE %s. Test %d\n", name, testNo);
        exit(EXIT_FAILURE);
    }
}

void assert(FILE *fp, char *msg, int statement)
{
    if (!statement)
    {
        if (fp != NULL)
        {
            fclose(fp);
        }

        fprintf(stderr, "FAILURE %s\n", msg);
        exit(EXIT_FAILURE);
    }
}

void assert_tv(FILE *fp, int testNo, char* name, int statement)
{
    char msg[32 + strlen(name)];
    sprintf(msg, "%s. Test %d", name, testNo);

    assert(fp, msg, statement);
}
