
# Loading additional proc with user specified bodies to compute parameter values.
source [file join [file dirname [file dirname [info script]]] gui/mux_v1_0.gtcl]

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "WIDTH"
  ipgui::add_param $IPINST -name "Data_Ports" -layout horizontal
  #Adding Group
  set Input_Bus_or_Signals [ipgui::add_group $IPINST -name "Input Bus or Signals" -display_name {Inputs: Bus or Signals} -layout horizontal]
  set_property tooltip {Inputs: Bus or Signals} ${Input_Bus_or_Signals}
  ipgui::add_param $IPINST -name "I0_BUS" -parent ${Input_Bus_or_Signals}
  ipgui::add_param $IPINST -name "I1_BUS" -parent ${Input_Bus_or_Signals}
  ipgui::add_param $IPINST -name "I2_BUS" -parent ${Input_Bus_or_Signals}
  ipgui::add_param $IPINST -name "I3_BUS" -parent ${Input_Bus_or_Signals}

  #Adding Group
  set Output_Bus_or_Signals [ipgui::add_group $IPINST -name "Output Bus or Signals" -display_name {Output: Bus or Signals} -layout horizontal]
  set_property tooltip {Output: Bus or Signals} ${Output_Bus_or_Signals}
  ipgui::add_param $IPINST -name "O_BUS" -parent ${Output_Bus_or_Signals}


}

proc update_PARAM_VALUE.I2_BUS { PARAM_VALUE.I2_BUS PARAM_VALUE.Data_Ports } {
	# Procedure called to update I2_BUS when any of the dependent parameters in the arguments change
	
	set I2_BUS ${PARAM_VALUE.I2_BUS}
	set Data_Ports ${PARAM_VALUE.Data_Ports}
	set values(Data_Ports) [get_property value $Data_Ports]
	if { [gen_USERPARAMETER_I2_BUS_ENABLEMENT $values(Data_Ports)] } {
		set_property enabled true $I2_BUS
	} else {
		set_property enabled false $I2_BUS
	}
}

proc validate_PARAM_VALUE.I2_BUS { PARAM_VALUE.I2_BUS } {
	# Procedure called to validate I2_BUS
	return true
}

proc update_PARAM_VALUE.I3_BUS { PARAM_VALUE.I3_BUS PARAM_VALUE.Data_Ports } {
	# Procedure called to update I3_BUS when any of the dependent parameters in the arguments change
	
	set I3_BUS ${PARAM_VALUE.I3_BUS}
	set Data_Ports ${PARAM_VALUE.Data_Ports}
	set values(Data_Ports) [get_property value $Data_Ports]
	if { [gen_USERPARAMETER_I3_BUS_ENABLEMENT $values(Data_Ports)] } {
		set_property enabled true $I3_BUS
	} else {
		set_property enabled false $I3_BUS
	}
}

proc validate_PARAM_VALUE.I3_BUS { PARAM_VALUE.I3_BUS } {
	# Procedure called to validate I3_BUS
	return true
}

proc update_PARAM_VALUE.Data_Ports { PARAM_VALUE.Data_Ports } {
	# Procedure called to update Data_Ports when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.Data_Ports { PARAM_VALUE.Data_Ports } {
	# Procedure called to validate Data_Ports
	return true
}

proc update_PARAM_VALUE.I0_BUS { PARAM_VALUE.I0_BUS } {
	# Procedure called to update I0_BUS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I0_BUS { PARAM_VALUE.I0_BUS } {
	# Procedure called to validate I0_BUS
	return true
}

proc update_PARAM_VALUE.I1_BUS { PARAM_VALUE.I1_BUS } {
	# Procedure called to update I1_BUS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.I1_BUS { PARAM_VALUE.I1_BUS } {
	# Procedure called to validate I1_BUS
	return true
}

proc update_PARAM_VALUE.O_BUS { PARAM_VALUE.O_BUS } {
	# Procedure called to update O_BUS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.O_BUS { PARAM_VALUE.O_BUS } {
	# Procedure called to validate O_BUS
	return true
}

proc update_PARAM_VALUE.WIDTH { PARAM_VALUE.WIDTH } {
	# Procedure called to update WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.WIDTH { PARAM_VALUE.WIDTH } {
	# Procedure called to validate WIDTH
	return true
}


proc update_MODELPARAM_VALUE.WIDTH { MODELPARAM_VALUE.WIDTH PARAM_VALUE.WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.WIDTH}] ${MODELPARAM_VALUE.WIDTH}
}

proc update_MODELPARAM_VALUE.I0_BUS { MODELPARAM_VALUE.I0_BUS PARAM_VALUE.I0_BUS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I0_BUS}] ${MODELPARAM_VALUE.I0_BUS}
}

proc update_MODELPARAM_VALUE.I1_BUS { MODELPARAM_VALUE.I1_BUS PARAM_VALUE.I1_BUS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I1_BUS}] ${MODELPARAM_VALUE.I1_BUS}
}

proc update_MODELPARAM_VALUE.I2_BUS { MODELPARAM_VALUE.I2_BUS PARAM_VALUE.I2_BUS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I2_BUS}] ${MODELPARAM_VALUE.I2_BUS}
}

proc update_MODELPARAM_VALUE.I3_BUS { MODELPARAM_VALUE.I3_BUS PARAM_VALUE.I3_BUS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.I3_BUS}] ${MODELPARAM_VALUE.I3_BUS}
}

