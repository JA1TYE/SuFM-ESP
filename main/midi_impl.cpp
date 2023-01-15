#include <cstdint>
#include <cstdio>

#include "./midi_lib/midi.h"
#include "midi_impl.h"
#include "./fm_lib/synth_ctrl.h"

namespace su_midi{
    midi_receiver_impl::midi_receiver_impl(su_synth::fm::synth_controller *synth_inst,std::uint16_t mask):midi_receiver_base(mask){
        synth_inst_ = synth_inst;
    }

    void midi_receiver_impl::note_on_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch){
        if(velocity == 0){
            synth_inst_->negate_tg(note,ch);  
        }
        else{
            synth_inst_->assert_tg(note,ch,velocity);
        }
        //printf("note on:%d,velocity:%d\n",note,velocity);
    }
    void midi_receiver_impl::note_off_handler(std::uint8_t note,std::uint8_t velocity,std::uint8_t ch){
        synth_inst_->negate_tg(note,ch);
        //printf("note off:%d,velocity:%d\n",note,velocity);
    }

    void midi_receiver_impl::pc_handler(std::uint8_t prog_num,std::uint8_t ch){
        synth_inst_->set_timbre(su_synth::fm::PROGRAM_CHANGE,prog_num,ch);
    }

    void midi_receiver_impl::cc_handler(std::uint8_t control_num,std::uint8_t value,std::uint8_t ch){
        if(control_num == CC_BANK_SELECT_MSB){
            synth_inst_->set_timbre(su_synth::fm::BANK_MSB,value,ch);
        }
        else if(control_num == CC_BANK_SELECT_LSB){
            synth_inst_->set_timbre(su_synth::fm::BANK_LSB,value,ch);
        }
        else if(control_num == CC_RESET_ALL_CONTROLLER){
            synth_inst_->reset(ch);
        }
        else if(control_num == CC_EXPRESSION_MSB){
            synth_inst_->set_expression(value,ch);
        }
        else if(control_num == CC_PAN_MSB){
            synth_inst_->set_panpot(value,ch);
        }
        else if(control_num == CC_CH_VOLUME_MSB){
            synth_inst_->set_volume(value,ch);
        }
        else if(control_num == CC_MODULATION_MSB){
            synth_inst_->set_modulation(su_synth::fm::MOD_WHEEL,value,ch);
        }
        else if(control_num == CC_FOOT_CONTROLLER_MSB){
            synth_inst_->set_modulation(su_synth::fm::MOD_FOOT,value,ch);
        }
        else if(control_num == CC_BREATH_CONTROLLER_MSB){
            synth_inst_->set_modulation(su_synth::fm::MOD_BREATH,value,ch);
        }
        else if(control_num == CC_DUMPER_PEDAL){
            synth_inst_->set_hold(value,ch);
        }
        else if(control_num == CC_RPN_MSB){
            synth_inst_->set_parameter_number(su_synth::fm::RPN_MSB,value,ch);
        }
        else if(control_num == CC_RPN_LSB){
            synth_inst_->set_parameter_number(su_synth::fm::RPN_LSB,value,ch);
        }
        else if(control_num == CC_NRPN_MSB){
            synth_inst_->set_parameter_number(su_synth::fm::NRPN_MSB,value,ch);
        }
        else if(control_num == CC_NRPN_LSB){
            synth_inst_->set_parameter_number(su_synth::fm::NRPN_LSB,value,ch);
        }
        else if(control_num == CC_DATA_ENRTY_MSB){
            synth_inst_->set_parameter_entry(true,value,ch);
        }
        else if(control_num == CC_DATA_ENRTY_LSB){
            //Note:Currently we ignore data entry LSB
            synth_inst_->set_parameter_entry(false,value,ch);
        }
    }

    void midi_receiver_impl::pitchbend_handler(std::int16_t value,std::uint8_t ch){
        synth_inst_->set_pitchbend(value,ch);
    }
}
