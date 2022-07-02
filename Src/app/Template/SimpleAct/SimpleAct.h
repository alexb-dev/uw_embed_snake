/*******************************************************************************
 * Copyright (C) Gallium Studio LLC. All rights reserved.
 *
 * This program is open source software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Alternatively, this program may be distributed and modified under the
 * terms of Gallium Studio LLC commercial licenses, which expressly supersede
 * the GNU General Public License and are specifically designed for licensees
 * interested in retaining the proprietary status of their code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact information:
 * Website - https://www.galliumstudio.com
 * Source repository - https://github.com/galliumstudio
 * Email - admin@galliumstudio.com
 ******************************************************************************/

#ifndef SIMPLE_ACT_H
#define SIMPLE_ACT_H

#include "qpcpp.h"
#include "fw_active.h"
#include "fw_timer.h"
#include "fw_evt.h"
#include "app_hsmn.h"

using namespace QP;
using namespace FW;

namespace APP {

class SimpleAct : public Active {
public:
    SimpleAct();

protected:
    static QState InitialPseudoState(SimpleAct * const me, QEvt const * const e);
    static QState Root(SimpleAct * const me, QEvt const * const e);
        static QState Stopped(SimpleAct * const me, QEvt const * const e);
        static QState Starting(SimpleAct * const me, QEvt const * const e);
        static QState Stopping(SimpleAct * const me, QEvt const * const e);
        static QState Started(SimpleAct * const me, QEvt const * const e);
        static QState Dead(SimpleAct * const me, QEvt const * const e);

    Evt m_inEvt;                // Static event copy of a generic incoming req to be confirmed. Added more if needed.
    Timer m_stateTimer;
    enum { POLL_TIMEOUT_MS = 1000, };
    Timer m_pollTimer;    


#define SIMPLE_ACT_TIMER_EVT \
    ADD_EVT(STATE_TIMER) \
    ADD_EVT(POLL_TIMER)

#define SIMPLE_ACT_INTERNAL_EVT \
    ADD_EVT(DONE) \
    ADD_EVT(FAILED) \
    ADD_EVT(DEAD) \
    ADD_EVT(TURN_N) \
    ADD_EVT(TURN_S) \
    ADD_EVT(TURN_W) \
    ADD_EVT(TURN_E) 

#undef ADD_EVT
#define ADD_EVT(e_) e_,

    enum {
        SIMPLE_ACT_TIMER_EVT_START = TIMER_EVT_START(SIMPLE_ACT),
        SIMPLE_ACT_TIMER_EVT
    };

    enum {
        SIMPLE_ACT_INTERNAL_EVT_START = INTERNAL_EVT_START(SIMPLE_ACT),
        SIMPLE_ACT_INTERNAL_EVT
    };

    class Failed : public ErrorEvt {
    public:
        Failed(Error error, Hsmn origin, Reason reason) :
            ErrorEvt(FAILED, error, origin, reason) {}
    };

    static const unsigned int MAX_LEN=30;
    static const unsigned int MIN_X=0;
    static const unsigned int MAX_X=11;
    static const unsigned int MIN_Y=0;
    static const unsigned int MAX_Y=13;
    struct Position{
        unsigned int x;
        unsigned int y;
    };
    enum Direction{N,S,E,W};

    unsigned int cur_len=6;
    Position snake[MAX_LEN]={{0,0},{0,1},{0,2},{0,3},{0,4},{0,5}};
    Direction cur_direction = Direction::W;
    bool need_new_apple=true;
    Position apple;
};
} // namespace APP

#endif // SIMPLE_ACT_H
