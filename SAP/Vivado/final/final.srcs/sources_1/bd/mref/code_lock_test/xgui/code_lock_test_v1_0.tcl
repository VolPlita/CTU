# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "C_CODE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_COUNTER_DELAY" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_FSM_TYPE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_TEST1" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_TEST2" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_TEST3" -parent ${Page_0}


}

proc update_PARAM_VALUE.C_CODE { PARAM_VALUE.C_CODE } {
	# Procedure called to update C_CODE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_CODE { PARAM_VALUE.C_CODE } {
	# Procedure called to validate C_CODE
	return true
}

proc update_PARAM_VALUE.C_COUNTER_DELAY { PARAM_VALUE.C_COUNTER_DELAY } {
	# Procedure called to update C_COUNTER_DELAY when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_COUNTER_DELAY { PARAM_VALUE.C_COUNTER_DELAY } {
	# Procedure called to validate C_COUNTER_DELAY
	return true
}

proc update_PARAM_VALUE.C_FSM_TYPE { PARAM_VALUE.C_FSM_TYPE } {
	# Procedure called to update C_FSM_TYPE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FSM_TYPE { PARAM_VALUE.C_FSM_TYPE } {
	# Procedure called to validate C_FSM_TYPE
	return true
}

proc update_PARAM_VALUE.C_TEST1 { PARAM_VALUE.C_TEST1 } {
	# Procedure called to update C_TEST1 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_TEST1 { PARAM_VALUE.C_TEST1 } {
	# Procedure called to validate C_TEST1
	return true
}

proc update_PARAM_VALUE.C_TEST2 { PARAM_VALUE.C_TEST2 } {
	# Procedure called to update C_TEST2 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_TEST2 { PARAM_VALUE.C_TEST2 } {
	# Procedure called to validate C_TEST2
	return true
}

proc update_PARAM_VALUE.C_TEST3 { PARAM_VALUE.C_TEST3 } {
	# Procedure called to update C_TEST3 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_TEST3 { PARAM_VALUE.C_TEST3 } {
	# Procedure called to validate C_TEST3
	return true
}


proc update_MODELPARAM_VALUE.C_CODE { MODELPARAM_VALUE.C_CODE PARAM_VALUE.C_CODE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_CODE}] ${MODELPARAM_VALUE.C_CODE}
}

proc update_MODELPARAM_VALUE.C_TEST1 { MODELPARAM_VALUE.C_TEST1 PARAM_VALUE.C_TEST1 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_TEST1}] ${MODELPARAM_VALUE.C_TEST1}
}

proc update_MODELPARAM_VALUE.C_TEST2 { MODELPARAM_VALUE.C_TEST2 PARAM_VALUE.C_TEST2 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_TEST2}] ${MODELPARAM_VALUE.C_TEST2}
}

proc update_MODELPARAM_VALUE.C_TEST3 { MODELPARAM_VALUE.C_TEST3 PARAM_VALUE.C_TEST3 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_TEST3}] ${MODELPARAM_VALUE.C_TEST3}
}

proc update_MODELPARAM_VALUE.C_FSM_TYPE { MODELPARAM_VALUE.C_FSM_TYPE PARAM_VALUE.C_FSM_TYPE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FSM_TYPE}] ${MODELPARAM_VALUE.C_FSM_TYPE}
}

proc update_MODELPARAM_VALUE.C_COUNTER_DELAY { MODELPARAM_VALUE.C_COUNTER_DELAY PARAM_VALUE.C_COUNTER_DELAY } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_COUNTER_DELAY}] ${MODELPARAM_VALUE.C_COUNTER_DELAY}
}

