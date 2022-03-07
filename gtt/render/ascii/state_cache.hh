#pragma once

#include <string>
#include <list>

namespace gtt {
namespace render {
namespace ascii {

class StateCache{
public:
  void
  register_new_state( std::string const & state ){
    // handle empty option
    if( all_states_.empty() ){
      all_states_.push_front( state );
      current_iter_ = all_states_.begin();
      return;
    }

    // skip non-updates
    if( *current_iter_ == state ) return;

    // delete future if there is any
    all_states_.remove( all_states_.begin(), current_iter_ );

    // save this state
    all_states_.push_front( state );
  }

  std::string
  get_current_state() const {
    if( current_iter_ != all_states_.end() ) {
      return *current_iter_;
    } else {
      return "";
    }
  }

  std::string
  undo(){
    if( current_iter_ != all_states_.end() ){
      ++current_iter_;
    }
    return *current_iter_;
  }

  std::string
  redo(){
    if( current_iter_ != all_states_.begin() ){
      --current_iter_;
    }
    return *current_iter_;
  }

private:
  // Last element is oldest state
  // First element is futuremost state
  std::list< std::string > all_states_;
  decltype(all_states_)::iterator current_iter_ = all_states_.begin();
};

} // ascii
} // render
} // gtt
