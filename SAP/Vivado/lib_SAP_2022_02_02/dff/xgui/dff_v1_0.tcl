# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "CE"
  ipgui::add_param $IPINST -name "Reset"

}

proc update_PARAM_VALUE.CE { PARAM_VALUE.CE } {
	# Procedure called to update CE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CE { PARAM_VALUE.CE } {
	# Procedure called to validate CE
	return true
}

proc update_PARAM_VALUE.Reset { PARAM_VALUE.Reset } {
	# Procedure called to update Reset when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.Reset { PARAM_VALUE.Reset } {
	# Procedure called to validate Reset
	return true
}


