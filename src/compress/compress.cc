/******************************************************************************
Copyright (c) 2019, Hubert GARAVEL
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <iostream>
#include "compress.hh"

namespace caesar {

/*---------------------------------------------------------------------------*/

compress::compress()
  : PREVIOUS_CELL('\0'), NB_REPEATED_CELLS(0)
{}

/*---------------------------------------------------------------------------*/

void compress::dump_compression(char CELL)
{
  /* assert (NB_REPEATED_CELLS == 0) <=> (PREVIOUS_CELL == '\0') */
  /* assert (PREVIOUS_CELL != '\n') */

  if (CELL == PREVIOUS_CELL) {
    /* assert (PREVIOUS_CELL != '\0') */
    ++NB_REPEATED_CELLS;
  } else {
    /* possibly emptying of the repetition buffer */
    if (NB_REPEATED_CELLS > 3) {
      std::cout << "(" << NB_REPEATED_CELLS << ")";
    } else if (NB_REPEATED_CELLS > 0) {
      for ( /* nothing */ ; NB_REPEATED_CELLS > 1; --NB_REPEATED_CELLS)
        printf ("%c", PREVIOUS_CELL);
    }
    std::cout << CELL;
    if (CELL == '\n') {
      /* particuliar case : end of line */
      PREVIOUS_CELL = '\0';
      NB_REPEATED_CELLS = 0;
    } else {
      /* case of an ordinary character */
      PREVIOUS_CELL = CELL;
      NB_REPEATED_CELLS = 1;
    }
  }
}

} // namespace caesar
