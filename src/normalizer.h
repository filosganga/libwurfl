/*
 * Copyright 2011 ff-dev.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Written by Filippo De Luca <me@filippodeluca.com>.  */

#ifndef NORMALIZER_H_
#define NORMALIZER_H_

#include <stdbool.h>

typedef struct _normalizer_t normalizer_t;

normalizer_t* normalizer_init();

void normalizer_free(normalizer_t* normalizer);

void normalizer_apply(normalizer_t* normalizer, char* dst, const char* src);


#endif /* NORMALIZER_H_ */
