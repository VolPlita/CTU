# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  ipgui::add_param $IPINST -name "width" -layout horizontal

}

proc update_PARAM_VALUE.LOGO_FILE { PARAM_VALUE.LOGO_FILE PARAM_VALUE.width } {
	# Procedure called to update LOGO_FILE when any of the dependent parameters in the arguments change
   set opvalue [get_property value ${PARAM_VALUE.width}]
   #puts "update_PARAM_VALUE"
   
   #100 MHz clock -> 4 - simulation (90 ns); 22 - synthesis (ca. 20 ms)
   if { $opvalue == 22 } { 
   # Synthesis
      set_property value "synthesis.png" ${PARAM_VALUE.LOGO_FILE}
      #puts "update_PARAM_VALUE 22"
   } else {   
   # Simulation
      set_property value "simulation.png" ${PARAM_VALUE.LOGO_FILE}
      #puts "update_PARAM_VALUE not 22"
   }
}

proc validate_PARAM_VALUE.LOGO_FILE { PARAM_VALUE.LOGO_FILE } {
	# Procedure called to validate LOGO_FILE
	return true
}

proc update_PARAM_VALUE.width { PARAM_VALUE.width } {
	# Procedure called to update width when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.width { PARAM_VALUE.width } {
	# Procedure called to validate width
	return true
}


proc update_MODELPARAM_VALUE.width { MODELPARAM_VALUE.width PARAM_VALUE.width } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.width}] ${MODELPARAM_VALUE.width}
}

