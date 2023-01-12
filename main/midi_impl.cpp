#include <cstdint>
#include <cstdio>
#include "./midi_lib/midi.h"
#include "midi_impl.h"
#include "./fm_lib/synth_ctrl.h"

namespace su_midi{
    void midi_receiver_impl::note_on_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch){
        if(velocity == 0){
            su_synth::synth_controller::negate_tg(note,ch);  
        }
        else{
            su_synth::synth_controller::assert_tg(note,ch,velocity);
        }
        //printf("note on:%d,velocity:%d\n",note,velocity);
    }
    void midi_receiver_impl::note_off_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch){
        su_synth::synth_controller::negate_tg(note,ch);
        //printf("note off:%d,velocity:%d\n",note,velocity);
    }

    void midi_receiver_impl::pc_handler(std::uint8_t prog_num,std::uint8_t ch){
        su_synth::synth_controller::set_timbre(su_synth::PROGRAM_CHANGE,prog_num,ch);
    }

    void midi_receiver_impl::cc_handler(std::uint8_t control_num,std::uint8_t value,std::uint8_t ch){
        if(control_num == CC_BANK_SELECT_MSB){
            su_synth::synth_controller::set_timbre(su_synth::BANK_MSB,value,ch);
        }
        else if(control_num == CC_BANK_SELECT_LSB){
            su_synth::synth_controller::set_timbre(su_synth::BANK_LSB,value,ch);
        }
        else if(control_num == CC_RESET_ALL_CONTROLLER){
            su_synth::synth_controller::reset(ch);
        }
        else if(control_num == CC_EXPRESSION_MSB){
            su_synth::synth_controller::set_expression(value,ch);
        }
        else if(control_num == CC_PAN_MSB){
            su_synth::synth_controller::set_panpot(value,ch);
        }
        else if(control_num == CC_CH_VOLUME_MSB){
            su_synth::synth_controller::set_volume(value,ch);
        }
        else if(control_num == CC_MODULATION_MSB){
            su_synth::synth_controller::set_modulation(su_synth::MOD_WHEEL,value,ch);
        }
        else if(control_num == CC_FOOT_CONTROLLER_MSB){
            su_synth::synth_controller::set_modulation(su_synth::MOD_FOOT,value,ch);
        }
        else if(control_num == CC_BREATH_CONTROLLER_MSB){
            su_synth::synth_controller::set_modulation(su_synth::MOD_BREATH,value,ch);
        }
        else if(control_num == CC_DUMPER_PEDAL){
            su_synth::synth_controller::set_hold(value,ch);
        }
        else if(control_num == CC_RPN_MSB){
            su_synth::synth_controller::set_parameter_number(su_synth::RPN_MSB,value,ch);
        }
        else if(control_num == CC_RPN_LSB){
            su_synth::synth_controller::set_parameter_number(su_synth::RPN_LSB,value,ch);
        }
        else if(control_num == CC_NRPN_MSB){
            su_synth::synth_controller::set_parameter_number(su_synth::NRPN_MSB,value,ch);
        }
        else if(control_num == CC_NRPN_LSB){
            su_synth::synth_controller::set_parameter_number(su_synth::NRPN_LSB,value,ch);
        }
        else if(control_num == CC_DATA_ENRTY_MSB){
            su_synth::synth_controller::set_parameter_entry(true,value,ch);
        }
        else if(control_num == CC_DATA_ENRTY_LSB){
            //Note:Currently we ignore data entry LSB
            su_synth::synth_controller::set_parameter_entry(false,value,ch);
        }
    }

    void midi_receiver_impl::pitchbend_handler(std::int16_t value,std::uint8_t ch){
        su_synth::synth_controller::set_pitchbend(value,ch);
    }
}
