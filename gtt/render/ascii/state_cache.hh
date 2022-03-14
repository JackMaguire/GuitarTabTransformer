#pragma once

#include <string>
#include <list>

namespace gtt {
namespace render {
namespace ascii {

class StateCache{
public:
  StateCache( std::string const & state ){
    all_states_.push_front( state );
    current_iter_ = all_states_.begin();
  }

  void
  register_new_state( std::string const & state );

  std::string
  get_current_state() const;

  std::string
  undo();

  std::string
  redo();

  auto size() const {
    return all_states_.size();
  }

private:
  // Last element is oldest state
  // First element is futuremost state
  std::list< std::string > all_states_;
  decltype(all_states_)::iterator current_iter_ = all_states_.begin();
};

void
StateCache::register_new_state( std::string const & state ){
  //Can't be empty

  // skip non-updates
  if( *current_iter_ == state ) return;

  // delete future if there is any
  all_states_.erase( all_states_.begin(), current_iter_ );

  // save this state
  all_states_.push_front( state );
  current_iter_ = all_states_.begin();
}

std::string
StateCache::get_current_state() const {
  if( current_iter_ != all_states_.end() ) {
    return *current_iter_;
  } else {
    return "";
  }
}

std::string
StateCache::undo(){
  if( current_iter_ != all_states_.end() and std::next(current_iter_) != all_states_.end() ){
    ++current_iter_;
  }
  return *current_iter_;
}

std::string
StateCache::redo(){
  if( current_iter_ != all_states_.begin() ){
    --current_iter_;
  }
  return *current_iter_;
}

} // ascii
} // render
} // gtt
