#!/usr/bin/env python3


from enum import Enum


# all states for linux updater, do NOT uses auto() since we DO care about the values.
class State(Enum):
    FAILED = 0
    SLEEP = 1
    PREUPDATE = 2
    UPDATE = 3
    REVERT = 4
    FORCE = 5


class UpdaterStateMachine():
    def __init__(self, init_state=State.SLEEP.value):
        self.__valid_transition = {
                State.FAILED.value : [State.FAILED.value, State.SLEEP.value, State.FORCE.value],
                State.SLEEP.value : [State.SLEEP.value, State.UPDATE.value, State.FORCE.value],
                State.PREUPDATE.value : [State.FAILED.value, State.SLEEP.value, State.PREUPDATE.value, State.UPDATE.vaue],
                State.UPDATE.value : [State.SLEEP.value, State.UPDATE.value, State.REVERT.value],
                State.REVERT.value : [State.SLEEP.value, State.REVERT.value],
                State.FORCE.value : [State.SLEEP.value, State.FAILED.value, State.FORCE.value]
                }

        # figure out initial state
        if new_state in self.__valid_transition:
            self._current_state = init_state
        else:
            self._current_state = State.FAILED.value


    def change_state(self, new_state):
        # (int) -> bool
        """ 
        Check if the state change is valid.
        Returns true if valid and transition happend
        Returns false if errored.
        """

        # check if new_state is a valid transition
        if new_state not in self.__valid_transition(self._current_state):
            self._current_state = State.FAILED.value
            return False # not a valid transition

        self._current_state = new_state
        return True


    @property
    def current_state(self):
        return self._current_state
