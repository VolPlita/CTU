
# Loading additional proc with user specified bodies to compute parameter values.
source [file join [file dirname [file dirname [info script]]] gui/xor_gate_v1_0.gtcl]

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "Inputs"
  #Adding Group
  set Inverted_inputs [ipgui::add_group $IPINST -name "Inverted inputs" -layout horizontal]
  ipgui::add_param $IPINST -name "I0_Inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I1_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I2_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I3_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I4_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I5_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I6_inverted" -parent ${Inverted_inputs}
  ipgui::add_param $IPINST -name "I7_inverted" -parent ${Inverted_inputs}

  #Adding Group
  set Inverted_Outputs [ipgui::add_group $IPINST -name "Inverted Outputs" -layout horizontal]
  ipgui::add_param $IPINST -name "O_inverted" -parent ${Inverted_Outputs}

}

proc update_PARAM_VALUE.I2_inverted { PARAM_VALUE.I2_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I2_inverted when any of the dependent parameters in the arguments change
	
	set I2_inverted ${PARAM_VALUE.I2_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I2_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I2_inverted
	} else {
		set_property enabled false $I2_inverted
		set_property value false $I2_inverted
	}
}

proc validate_PARAM_VALUE.I2_inverted { PARAM_VALUE.I2_inverted } {
	# Procedure called to validate I2_inverted
	return true
}

proc update_PARAM_VALUE.I3_inverted { PARAM_VALUE.I3_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I3_inverted when any of the dependent parameters in the arguments change
	
	set I3_inverted ${PARAM_VALUE.I3_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I3_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I3_inverted
	} else {
		set_property enabled false $I3_inverted
		set_property value false $I3_inverted
	}
}

proc validate_PARAM_VALUE.I3_inverted { PARAM_VALUE.I3_inverted } {
	# Procedure called to validate I3_inverted
	return true
}

proc update_PARAM_VALUE.I4_inverted { PARAM_VALUE.I4_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I4_inverted when any of the dependent parameters in the arguments change
	
	set I4_inverted ${PARAM_VALUE.I4_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I4_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I4_inverted
	} else {
		set_property enabled false $I4_inverted
		set_property value false $I4_inverted
	}
}

proc validate_PARAM_VALUE.I4_inverted { PARAM_VALUE.I4_inverted } {
	# Procedure called to validate I4_inverted
	return true
}

proc update_PARAM_VALUE.I5_inverted { PARAM_VALUE.I5_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I5_inverted when any of the dependent parameters in the arguments change
	
	set I5_inverted ${PARAM_VALUE.I5_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I5_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I5_inverted
	} else {
		set_property enabled false $I5_inverted
		set_property value false $I5_inverted
	}
}

proc validate_PARAM_VALUE.I5_inverted { PARAM_VALUE.I5_inverted } {
	# Procedure called to validate I5_inverted
	return true
}

proc update_PARAM_VALUE.I6_inverted { PARAM_VALUE.I6_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I6_inverted when any of the dependent parameters in the arguments change
	
	set I6_inverted ${PARAM_VALUE.I6_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I6_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I6_inverted
	} else {
		set_property enabled false $I6_inverted
		set_property value false $I6_inverted
	}
}

proc validate_PARAM_VALUE.I6_inverted { PARAM_VALUE.I6_inverted } {
	# Procedure called to validate I6_inverted
	return true
}

proc update_PARAM_VALUE.I7_inverted { PARAM_VALUE.I7_inverted PARAM_VALUE.Inputs } {
	# Procedure called to update I7_inverted when any of the dependent parameters in the arguments change
	
	set I7_inverted ${PARAM_VALUE.I7_inverted}
	set Inputs ${PARAM_VALUE.Inputs}
	set values(Inputs) [get_property value $Inputs]
	if { [gen_USERPARAMETER_I7_inverted_ENABLEMENT $values(Inputs)] } {
		set_property enabled true $I7_inverted
	} else {
		set_property enabled false $I7_inverted
		set_property value false $I7_inverted
	}
}

proc validate_PARAM_VALUE.I7_inverted { PARAM_VALUE.I7_inverted } {
	# Procedure called to validate I7_inverted
	return true
}

proc update_PARAM_VALUE.I0_Inverted { PARAM_VALUE.I0_Inverted } {
	# Procedure called to update I0_Inverted when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I0_Inverted { PARAM_VALUE.I0_Inverted } {
	# Procedure called to validate I0_Inverted
	return true
}

proc update_PARAM_VALUE.I1_inverted { PARAM_VALUE.I1_inverted } {
	# Procedure called to update I1_inverted when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I1_inverted { PARAM_VALUE.I1_inverted } {
	# Procedure called to validate I1_inverted
	return true
}

proc update_PARAM_VALUE.Inputs { PARAM_VALUE.Inputs } {
	# Procedure called to update Inputs when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.Inputs { PARAM_VALUE.Inputs } {
	# Procedure called to validate Inputs
	return true
}

proc update_PARAM_VALUE.O_inverted { PARAM_VALUE.O_inverted } {
	# Procedure called to update O_inverted when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.O_inverted { PARAM_VALUE.O_inverted } {
	# Procedure called to validate O_inverted
	return true
}


proc update_MODELPARAM_VALUE.I0_inverted { MODELPARAM_VALUE.I0_inverted PARAM_VALUE.I0_Inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I0_Inverted}] ${MODELPARAM_VALUE.I0_inverted}
}

proc update_MODELPARAM_VALUE.I1_inverted { MODELPARAM_VALUE.I1_inverted PARAM_VALUE.I1_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I1_inverted}] ${MODELPARAM_VALUE.I1_inverted}
}

proc update_MODELPARAM_VALUE.I2_inverted { MODELPARAM_VALUE.I2_inverted PARAM_VALUE.I2_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I2_inverted}] ${MODELPARAM_VALUE.I2_inverted}
}

proc update_MODELPARAM_VALUE.I3_inverted { MODELPARAM_VALUE.I3_inverted PARAM_VALUE.I3_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I3_inverted}] ${MODELPARAM_VALUE.I3_inverted}
}

proc update_MODELPARAM_VALUE.I4_inverted { MODELPARAM_VALUE.I4_inverted PARAM_VALUE.I4_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I4_inverted}] ${MODELPARAM_VALUE.I4_inverted}
}

proc update_MODELPARAM_VALUE.I5_inverted { MODELPARAM_VALUE.I5_inverted PARAM_VALUE.I5_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I5_inverted}] ${MODELPARAM_VALUE.I5_inverted}
}

proc update_MODELPARAM_VALUE.I6_inverted { MODELPARAM_VALUE.I6_inverted PARAM_VALUE.I6_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I6_inverted}] ${MODELPARAM_VALUE.I6_inverted}
}

proc update_MODELPARAM_VALUE.I7_inverted { MODELPARAM_VALUE.I7_inverted PARAM_VALUE.I7_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I7_inverted}] ${MODELPARAM_VALUE.I7_inverted}
}

proc update_MODELPARAM_VALUE.O_inverted { MODELPARAM_VALUE.O_inverted PARAM_VALUE.O_inverted } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.O_inverted}] ${MODELPARAM_VALUE.O_inverted}
}

