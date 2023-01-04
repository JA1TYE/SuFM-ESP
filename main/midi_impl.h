#ifndef _MIDI_IMPL_H_
#define _MIDI_IMPL_H_

#include <cstdint>
#include "synth_config.h"
#include "midi.h"

namespace su_midi{
    class midi_receiver_impl:public midi_receiver_base{
        protected:
            void note_on_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch);
            void note_off_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch);
            void pc_handler(std::uint8_t prog_num,std::uint8_t ch);
            void cc_handler(std::uint8_t control_num,std::uint8_t value,std::uint8_t ch);
            void pitchbend_handler(std::int16_t value,std::uint8_t ch);
        public:
            std::uint16_t receive_ch_flag = ~(1 << 9);//CH1 only, 0xffff -> all channels
    };
}

#endif