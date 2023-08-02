proc sortPorts { ports &bus &bit } {

  upvar ${&bus} busPorts
  upvar ${&bit} bitPorts
  array set busPorts [list]
  set bitPorts [list]

  foreach port $ports {
    set name [get_property -quiet NAME [get_bd_ports $port]]
	set busStart [get_property -quiet LEFT [get_bd_ports $port]]
    set busStop  [get_property -quiet RIGHT  [get_bd_ports $port]]
    if {$busStart != {}} {
    #   set busWidth [get_property -quiet BUS_WIDTH [get_ports $port]]
      array set busPorts [list $name [list $busStop $busStart]]
    } else {
      lappend bitPorts $name
    }
  }
  return 0
}

proc sortPins { pins &bus &bit } {

  upvar ${&bus} busPins
  upvar ${&bit} bitPins
  array set busPins [list]
  array set bitPins [list]

  foreach pin $pins {
    set name [get_property -quiet NAME [get_bd_pins $pin]]
	set dir [get_property -quiet DIR [get_bd_pins $pin]]
	set busStart [get_property -quiet LEFT [get_bd_pins $pin]]
    set busStop  [get_property -quiet RIGHT  [get_bd_pins $pin]]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins $pin]]
	if {$net != ""} {
		set net_name [get_property -quiet NAME $net]
	} else {
		if {$dir == "I"} {
			if {$busStart != {}} {
				set net_name "(others => '0')"
				catch {common::send_msg_id "bd2vhdl    pin-2" "CRITICAL WARNING" "${pin}: nepripojen, bude pripojen k '0'"}
			} else {
				set net_name "'0'"
				catch {common::send_msg_id "bd2vhdl    pin-2" "CRITICAL WARNING" "${pin}: nepripojen, bude pripojen k '0'"}
			}
		} else {
			set net_name "open"
			catch {common::send_msg_id "bd2vhdl    pin-3" "CRITICAL WARNING" "${pin}: pin odstranen, vystup neni pripojen!"}
		}
	}
	if {$busStart != {}} {
		array set busPins [list $name [list $busStop $busStart $net_name]]
	} else {
		array set bitPins [list $name $net_name]
	}
  }
  return 0
}

proc sortNets { nets &bus &bit } {

  upvar ${&bus} busNets
  upvar ${&bit} bitNets
  array set busNets [list ]
  set bitNets [list]

  foreach net $nets {
    set name [get_property -quiet NAME [get_bd_nets $net]]
	set pin [lindex [get_bd_pins -of_objects [get_bd_nets $net]] 0]
	set busStart [get_property -quiet LEFT [get_bd_pins $pin]]
    set busStop  [get_property -quiet RIGHT  [get_bd_pins $pin]]
    if {$busStart != {}} {
    #   set busWidth [get_property -quiet BUS_WIDTH [get_ports $port]]
      array set busNets [list $name [list $busStop $busStart]]
    } else {
      lappend bitNets $name
    }
  }
  return 0
}

proc array2sortedList { &ar } {

  upvar ${&ar} ar
  set sortedList [list]
  foreach key [lsort -dictionary [array names ar]] {
    lappend sortedList $key
    lappend sortedList $ar($key)
  }
  return $sortedList
}

proc getInPinNet { cell pin default_value} {
	set pin [get_bd_pins -quiet $pin]
	set net [get_bd_nets -quiet -of_objects $pin]
	if {$net == ""} {
		catch {common::send_msg_id "bd2vhdl    pin-2" "CRITICAL WARNING" "${pin} nepripojen, bude pripojeno k $default_value"}
		set net_name $default_value
	} else {
		set net_name [get_property NAME $net]
	}
	return $net_name
}

proc getGate { cell  function} {
	set inputs_count [get_property CONFIG.Inputs [get_bd_cells -quiet $cell]]
	set content {}
	set prom {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]
	for {set i 0} { $i < $inputs_count } { incr i } {
		set cfg [format "CONFIG.I%u_Inverted" $i]
		set pin [format "%s/I%u" $cell $i]
		set inverted [get_property $cfg  [get_bd_cells -quiet $cell]]
		#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
		set net_name [getInPinNet $cell $pin "\'0\'"]
		#set net_name [get_property NAME $net]
		if {$inverted == "true"} {
			append prom [format "not(%s)" $net_name]
		} else {
			append prom [format "%s" $net_name]
		}
		if {$i < ($inputs_count-1)} {
			append prom [format " %s " $function]
		}
	}
	set cfg [format "CONFIG.O_Inverted" $i]
	set pin [format "%s/O" $cell]
	set inverted [get_property $cfg  [get_bd_cells -quiet $cell]]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set out_name [get_property NAME $net]
		if {$inverted == "true"} {
			append content [format "\t%s <= not(%s);\n" $out_name $prom];	
		} else {
			append content [format "\t%s <= %s;\n" $out_name $prom];
		}
		return $content
	} else {
		catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, vystup neni pripojen!"}
		return [append content "\t-- odstraneno, vystup neni pripojen\n"]
	}
}

proc getBuf { cell  function } {
	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set pin [format "%s/I" $cell]
	#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
	#set net_name [get_property NAME $net]
	set net_name [getInPinNet $cell $pin "\'0\'"]

	set pin [format "%s/O" $cell]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set out_name [get_property NAME $net]
		if {$function == true} {
			append content [format "\t%s <= not(%s);\n" $out_name $net_name];	
		} else {
			append content  [format "\t%s <= %s;\n" $out_name $net_name];
		}
		return $content
	} else {
		catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, vystup neni pripojen!"}
		return [append content "\t-- odstraneno, vystup neni pripojen\n"]
	}
}

proc getSource { cell  function } {
	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set pin [format "%s/O" $cell]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set out_name [get_property NAME $net]
		if {$function == true} {
			append content [format "\t%s <= '1';\n" $out_name];	
		} else {
			append content [format "\t%s <= '0';\n" $out_name];
		}
		return $content
	} else {
		catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, vystup neni pripojen!"}
		return [append content "\t-- odstraneno, vystup neni pripojen\n"]
	}
}

proc getSplitter { cell } {
	set width [get_property CONFIG.WIDTH [get_bd_cells -quiet $cell]]
	set pin [format "%s/I" $cell]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set in_name [get_property NAME $net]
		set content {}
		set name [get_property NAME $cell]
		append content [format "\n\t-- %s\n" $name]
		for {set i 0} { $i < $width } { incr i } {
			set pin [format "%s/O%u" $cell $i]
			set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
			if {$net != ""} {
				set net_name [get_property NAME $net]
				append content [format "\t%s <= %s(%u);\n" $net_name $in_name $i]
			} else {
				catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: ${pin} odstranen, vystup neni pripojen!"}
			}
		}
	} else {
		set content {}
		set name [get_property NAME $cell]
		append content [format "\n\t-- %s\n" $name]
		for {set i 0} { $i < $width } { incr i } {
			set pin [format "%s/O%u" $cell $i]
			set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
			if {$net != ""} {
				set net_name [get_property NAME $net]
				append content [format "\t%s <= \'0\';\n" $net_name $i]
				catch {common::send_msg_id "bd2vhdl  jadro-2" "CRITICAL WARNING" "${cell} nepripojen vstup, ${net_name} bude pripojeno k \'0\'"}
			} else {
				catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: ${pin} odstranen, vystup neni pripojen!"}
			}
		}
	}
	return $content
}

proc getDff { cell } {
	set ce [get_property CONFIG.CE [get_bd_cells -quiet $cell]]
	set reset [get_property CONFIG.Reset [get_bd_cells -quiet $cell]]

	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set pin [format "%s/q" $cell]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set out_name [get_property NAME $net]

		set pin [format "%s/clk" $cell]
		set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
		if {$net == ""} {
			catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, nejsou pripojene hodiny!"}
			return [append content "\t-- odstraneno, , nejsou pripojene hodiny!\n"]
		}
		#set clk_name [get_property NAME $net]
		set clk_name [getInPinNet $cell $pin "\'0\'"]

		set pin [format "%s/d" $cell]
		#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
		#set in_name [get_property NAME $net]
		set in_name [getInPinNet $cell $pin "\'0\'"]

		if {$ce == "true"} {
			set pin [format "%s/ce" $cell]
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set ce_name [get_property NAME $net]
			set ce_name [getInPinNet $cell $pin "\'1\'"]
		}

		if {$reset == "true"} {
			set pin [format "%s/reset" $cell]
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set reset_name [get_property NAME $net]
			set reset_name [getInPinNet $cell $pin "\'0\'"]
		}

		append content [format "\tprocess (%s)\n\tbegin\n" $clk_name]
		append content [format "\t\tif rising_edge(%s) then\n" $clk_name]
		if {$reset == "true"} {
			append content [format "\t\t\tif %s = '1' then\n\t\t\t\t%s <= '0';\n\t\t\tels" $reset_name $out_name]
		} else {
			append content "\t\t\t"
		}

		if {$ce == "true"} {
			append content [format "if %s = '1' then\n" $ce_name]
			append content [format "\t\t\t\t%s <= %s;\n" $out_name $in_name]
			append content "\t\t\tend if;\n"
		} else {
			if {$reset == "true"} {
				append content [format "e\n\t\t\t\t%s <= %s;\n" $out_name $in_name]
				append content "\t\t\tend if;\n"
			} else {
				append content [format "%s <= %s;\n" $out_name $in_name]
			}
		}

		append content "\t\tend if;\n\tend process;\n";
	} else {
		catch {common::send_msg_id "bd2vhdl jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, vystup neni pripojen!"}
		return [append content "\t-- odstraneno, vystup neni pripojen\n"]
	}
	return $content
}

proc getMux { cell } {
	set ports_count [get_property CONFIG.Data_Ports [get_bd_cells -quiet $cell]]
	set width [get_property CONFIG.WIDTH [get_bd_cells -quiet $cell]]
	set i0_bus [get_property CONFIG.I0_BUS [get_bd_cells -quiet $cell]]
	set i1_bus [get_property CONFIG.I1_BUS [get_bd_cells -quiet $cell]]
	set i2_bus [get_property CONFIG.I2_BUS [get_bd_cells -quiet $cell]]
	set i3_bus [get_property CONFIG.I3_BUS [get_bd_cells -quiet $cell]]
	set o_bus [get_property CONFIG.O_BUS [get_bd_cells -quiet $cell]]

	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	for {set i 0} { $i < $width } { incr i } {
		if {$o_bus == "true"} {
			set pin [format "%s/O" $cell]
			set vecO [format "(%u)" $i]
		} else {
			set pin [format "%s/O_%u" $cell $i]
			set vecO ""
		}
		set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
		if {$net != ""} {
			set out_name [get_property NAME $net]

			if {$i0_bus == "true"} {
				set pin [format "%s/I0" $cell]
				set vec0 [format "(%u)" $i]
			} else {
				set pin [format "%s/I0_%u" $cell $i]
				set vec0 ""
			}
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set i0_name [get_property NAME $net]
			set i0_name [getInPinNet $cell $pin "\'0\'"]
			if {$i0_name == "'0'"} {
				set vec0 ""
			}

			if {$i1_bus == "true"} {
				set pin [format "%s/I1" $cell]
				set vec1 [format "(%u)" $i]
			} else {
				set pin [format "%s/I1_%u" $cell $i]
				set vec1 ""
			}
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set i1_name [get_property NAME $net]
			set i1_name [getInPinNet $cell $pin "\'0\'"]
			if {$i1_name == "'0'"} {
				set vec1 ""
			}

			set pin [format "%s/Sel0" $cell]
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set sel0_name [get_property NAME $net]
			set sel0_name [getInPinNet $cell $pin "\'0\'"]

			if {$ports_count > 2} {
				if {$i2_bus == "true"} {
					set pin [format "%s/I2" $cell]
					set vec2 [format "(%u)" $i]
				} else {
					set pin [format "%s/I2_%u" $cell $i]
					set vec2 ""
				}
				#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
				#set i2_name [get_property NAME $net]
				set i2_name [getInPinNet $cell $pin "\'0\'"]
				if {$i2_name == "'0'"} {
					set vec2 ""
				}

				if {$i3_bus == "true"} {
					set pin [format "%s/I3" $cell]
					set vec3 [format "(%u)" $i]
				} else {
					set pin [format "%s/I3_%u" $cell $i]
					set vec3 ""
				}
				#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
				#set i3_name [get_property NAME $net]
				set i3_name [getInPinNet $cell $pin "\'0\'"]
				if {$i3_name == "'0'"} {
					set vec3 ""
				}

				set pin [format "%s/Sel1" $cell]
				#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
				#set sel1_name [get_property NAME $net]
				set sel1_name [getInPinNet $cell $pin "\'0\'"]
				
				if {$sel0_name != "'0'" && $sel1_name != "'0'"} {
					append content [format "\t%s%s <= %s%s when %s = '0' and %s = '0' else\n" $out_name $vecO $i0_name $vec0 $sel1_name $sel0_name]
					append content [format "\t\t%s%s when %s = '0' and %s = '1' else\n" $i1_name $vec1 $sel1_name $sel0_name]
					append content [format "\t\t%s%s when %s = '1' and %s = '0' else\n" $i2_name $vec2 $sel1_name $sel0_name]
					append content [format "\t\t%s%s when %s = '1' and %s = '1' else '0';\n\n" $i3_name $vec3 $sel1_name $sel0_name]
				} else {
					append content [format "\t%s%s <= %s%s;\n" $out_name $vecO $i0_name $vec0]
				}
			} else {
				if {$sel0_name != "'0'"} {
					append content [format "\t%s%s <= %s%s when (%s = '1') else %s%s;\n" $out_name $vecO $i1_name $vec1 $sel0_name $i0_name $vec0]
				} else {
					append content [format "\t%s%s <= %s%s;\n" $out_name $vecO $i0_name $vec0]
				}
			}
		} else {
			catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" " ${cell}: pin ${pin} odstranen, vystup neni pripojen!"}
			return [append content "\t-- ${pin} odstranen, vystup neni pripojen\n"]
		}
		
	}
	return $content
}

proc getDebounce { cell } {
	set width [get_property CONFIG.width [get_bd_cells -quiet $cell]]

	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set pin [format "%s/tl_out" $cell]
	set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
	if {$net != ""} {
		set tl_out [get_property NAME $net]

		set pin [format "%s/clk" $cell]
		set net [get_bd_nets -quiet -of_objects [get_bd_pins -quiet $pin]]
		if {$net != ""} {
			set clk [get_property NAME $net]

			set pin [format "%s/tl_in" $cell]
			#set net [get_bd_nets -of_objects [get_bd_pins -quiet $pin]]
			#set tl_in [get_property NAME $net]
			set tl_in [getInPinNet $cell $pin "\'0\'"]

			append content [ format "\t%s : debounce\n\t\tgeneric map (\n\t\t\twidth => %s\n\t\t)\n\t\tport map (\n\t\t\tclk	=> %s,\n\t\t\ttl_in	=> %s,\n\t\t\ttl_out	=> %s\n\t\t);\n" $name $width $clk $tl_in $tl_out]
		} else {
			catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, nejsou pripojene hodiny!"}
			return [append content "\t-- odstraneno, , nejsou pripojene hodiny!\n"]	
		}
	} else {
		catch {common::send_msg_id "bd2vhdl  jadro-3" "CRITICAL WARNING" "${cell}: odstraneno, vystup neni pripojen!"}
		return [append content "\t-- odstraneno, vystup neni pripojen\n"]
	}
	
	return $content
}

set debounce_component "\tcomponent debounce
\t\tgeneric (
\t\t\twidth: integer := 22
\t\t);
\t\tport (
\t\t\tclk    : in  std_logic;
\t\t\ttl_in  : in  std_logic;
\t\t\ttl_out : out std_logic
\t\t);
\tend component;\n"

proc getModule_ref { cell components component_name} {
	upvar $components comps

	set inPins [lsort [get_bd_pins -quiet -of_objects [get_bd_cells $cell] -filter DIR==I]]
	array set inputBusPins [list]
	array set inputBitPins [list]
	sortPins $inPins inputBusPins inputBitPins

	set outPins [lsort [get_bd_pins -quiet -of_objects [get_bd_cells $cell] -filter DIR==O]]
	array set outputBusPins [list]
	array set outputBitPins [list]
	sortPins $outPins outputBusPins outputBitPins

	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set lines [list]
	foreach {pin net} [array2sortedList inputBitPins] {
    	lappend lines [list "$pin" "in  std_logic;" "$net"]
  	}
	foreach {pin busInfo} [array2sortedList inputBusPins] {
		lassign $busInfo stop start net
		if {$start>$stop} {
			lappend lines [list "$pin" "in  std_logic_vector($start downto $stop);" "$net"]
		} else {
			lappend lines [list "$pin" "in  std_logic_vector($start to $stop);" "$net"]
		}
  	}
	foreach {pin net} [array2sortedList outputBitPins] {
    	lappend lines [list "$pin" "out std_logic;" "$net"]
	}
	foreach {pin busInfo} [array2sortedList outputBusPins] {
		lassign $busInfo stop start net
		if {$start>$stop} {
			lappend lines [list "$pin"  "out std_logic_vector($start downto $stop);" "$net"]
		} else {
			lappend lines [list "$pin" "out std_logic_vector($start to $stop);" "$net"]
		}
	}
	# Detect maximum column width to align columns
	foreach line $lines {
		set width [string length [lindex $line 0]]
		if {![info exist maxWidth] || $maxWidth < $width} {
			set maxWidth $width
		}
	}
	set ports {}
	set port_map {}
	foreach line $lines {
		append ports [format "\n\t\t\t%-${maxWidth}s :  %-${maxWidth}s" [lindex $line 0] [lindex $line 1]]
		append port_map [format "\n\t\t\t%-${maxWidth}s =>  %-${maxWidth}s," [lindex $line 0] [lindex $line 2]]
	}
	# Remove the last semi-colon
  	set index [string last {;} $ports]
  	set ports [string replace $ports $index $index {}]
	# Remove the last ,
  	set index [string last {,} $port_map]
  	set port_map [string replace $port_map $index $index {}]

	# nepridavat pokud uz tam ta komponenta je
	if {[string first $component_name $comps] < 0} {
		append comps "\tcomponent ${component_name}\n\t\tport ("
		append comps $ports
		append comps "\n\t\t);\n\tend component;\n\n"
	}
	append content "\tinst_${name} : ${component_name}\n"
	append content "\t\tport map ("
	append content $port_map
	append content "\n\t\t);\n"
}

proc getHier { cell debounce components modules} {
	global debounce_component
	upvar $components comps
	upvar $modules mods
	upvar $debounce debs

	set inPins [lsort [get_bd_pins -quiet -of_objects [get_bd_cells $cell] -filter DIR==I]]
	array set inputBusPins [list]
	array set inputBitPins [list]
	sortPins $inPins inputBusPins inputBitPins

	set outPins [lsort [get_bd_pins -quiet -of_objects [get_bd_cells $cell] -filter DIR==O]]
	array set outputBusPins [list]
	array set outputBitPins [list]
	sortPins $outPins outputBusPins outputBitPins

	set content {}
	set name [get_property NAME $cell]
	append content [format "\n\t-- %s\n" $name]

	set lines [list]
	foreach {pin net} [array2sortedList inputBitPins] {
    	lappend lines [list "$pin" "in  std_logic;" "$net"]
  	}
	foreach {pin busInfo} [array2sortedList inputBusPins] {
		lassign $busInfo stop start net
		if {$start>$stop} {
			lappend lines [list "$pin" "in  std_logic_vector($start downto $stop);" "$net"]
		} else {
			lappend lines [list "$pin" "in  std_logic_vector($start to $stop);" "$net"]
		}
  	}
	foreach {pin net} [array2sortedList outputBitPins] {
    	lappend lines [list "$pin" "out std_logic;" "$net"]
	}
	foreach {pin busInfo} [array2sortedList outputBusPins] {
		lassign $busInfo stop start net
		if {$start>$stop} {
			lappend lines [list "$pin"  "out std_logic_vector($start downto $stop);" "$net"]
		} else {
			lappend lines [list "$pin" "out std_logic_vector($start to $stop);" "$net"]
		}
	}
	# Detect maximum column width to align columns
	foreach line $lines {
		set width [string length [lindex $line 0]]
		if {![info exist maxWidth] || $maxWidth < $width} {
			set maxWidth $width
		}
	}
	set ports {}
	set port_map {}
	foreach line $lines {
		append ports [format "\n\t\t\t%-${maxWidth}s :  %-${maxWidth}s" [lindex $line 0] [lindex $line 1]]
		append port_map [format "\n\t\t\t%-${maxWidth}s =>  %-${maxWidth}s," [lindex $line 0] [lindex $line 2]]
	}
	# Remove the last semi-colon
  	set index [string last {;} $ports]
  	set ports [string replace $ports $index $index {}]
	# Remove the last ,
  	set index [string last {,} $port_map]
  	set port_map [string replace $port_map $index $index {}]

	set filtr [format {PATH !~ "%s/.*/.*"} $cell]
	set nets [lsort [get_bd_nets -quiet -of_objects [get_bd_cells $cell] -regexp -filter $filtr]]
	array set busNets [list ]
	set bitNets [list]
	sortNets $nets busNets bitNets

	set lines [list]
	# Process single bit nets
	foreach net [lsort -dictionary $bitNets] {
		lappend lines [list "$net" "  std_logic;"]
	}
	# Process bus nets
	foreach {net busInfo} [array2sortedList busNets] {
		lassign $busInfo stop start
		if {$start>$stop} {
		lappend lines [list "$net" "  std_logic_vector($start downto $stop);"]
		} else {
		lappend lines [list "$net" "  std_logic_vector($start to $stop);"]
		}
	}

	# Detect maximum column width to align columns
	foreach line $lines {
		set width [string length [lindex $line 0]]
		if {![info exist maxWidth] || $maxWidth < $width} {
			set maxWidth $width
		}
	}

	# Build the content of the stub:
	set signals {}
	foreach line $lines {
		append signals [format "\n\tsignal %-${maxWidth}s :  %-${maxWidth}s" [lindex $line 0] [lindex $line 1]]
	}
  set ports2Signals {}
    foreach net [lsort -dictionary $nets] {
        set net_name [get_property NAME [get_bd_nets -quiet $net]]
        set ports_b [get_bd_ports -quiet -of_objects [get_bd_nets -quiet $net]]
        if {$ports == {}} {
            break
        }
        foreach port [lsort -dictionary $ports_b] {
            set port_name [get_property NAME [get_bd_pins -quiet $port]]
            set dir [get_property DIR [get_bd_pins -quiet $port]]
            if {$dir == "I"} {
                append ports2Signals "\t$net_name <= $port_name;\n"    
            } else {
                append ports2Signals "\t$port_name <= $net_name;\n"
            }
        }
    }
  
	set debounce_inner false
  	set components_inner {}
  	set modules_inner {}
  	set body [getIPs $cell debounce_inner components_inner modules_inner]
	if {$debounce_inner} {
		puts "######################################################################x"
		append components_inner $debounce_component
		set debs true
    }
	
	set comp_name [string replace $cell 0 0 {}]
    set comp_name [string map {/ _} $comp_name]
    append comps "\tcomponent ${comp_name}\n\t\tport ("
    append mods "\n\nlibrary IEEE;\nuse IEEE.std_logic_1164.all;\n\n"
    append mods "entity ${comp_name} is\n\tport("
    append mods $ports
    append comps $ports
    append mods "\n\t\t);\nend entity ${comp_name};\n\narchitecture Behavioral of ${comp_name} is\n"
    append mods $components_inner
    append mods $signals
    append mods "\nbegin\n"
    append mods $ports2Signals
    append mods $body
    append mods "\nend Behavioral;"
    append comps "\n\t\t);\n\tend component;\n\n"
    append content "\tinst_${comp_name} : ${comp_name}\n"
    append content "\t\tport map ("
    append content $port_map
    append content "\n\t\t);\n"
    append mods $modules_inner

	return $content
}


proc getIPs { parent debounce components modules} {
  upvar $debounce deb
  upvar $components comps
  upvar $modules mods
  if {$parent == ""} {
	set cells [get_bd_cells]	  
  } else {
	set filtr [format {PATH !~ "%s/.*/.*"} $parent]
	set cells [get_bd_cells -of_objects [get_bd_cells $parent] -regexp -filter $filtr]
  }
  
  set content {}
  foreach cell [lsort -dictionary $cells] {
	set type [get_property TYPE [get_bd_cells -quiet $cell]]
	set name [get_property NAME [get_bd_cells -quiet $cell]]
	set vlnv [get_property VLNV [get_bd_cells -quiet $cell]]
	if {$type == "ip"} {
		set vlnvs [split $vlnv ":"]
		if {[lindex $vlnvs 0] == "FIT"} {
		 	switch [lindex $vlnvs 2] {
				"or" {
					append content [getGate $cell "or"]
				}
				"and" {
					append content [getGate $cell "and"]
				}
				"xor" {
					append content [getGate $cell "xor"]
				}
				"buf" {
					append content [getBuf $cell false]
				}
				"inv" {
					append content [getBuf $cell true]
				}
				"gnd" {
					append content [getSource $cell false]
				}
				"vcc" {
					append content [getSource $cell true]
				}
				"splitter" {
					append content [getSplitter $cell]
				}
				"mux" {
					append content [getMux $cell]
				}
				"dff" {
					append content [getDff $cell]
				}
				"debounce" {
					append content [getDebounce $cell]
					set deb true
				}
			}
		} elseif {[lindex $vlnvs 0] == "xilinx.com" && [lindex $vlnvs 1] == "module_ref"} {
			append content [getModule_ref $cell comps [lindex $vlnvs 2]]
		}
			
	} elseif {$type == "hier"} {
		append content [getHier $cell deb comps mods]
	}
  }
  return $content
}


set debounce_code "\n\n\nlibrary IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity debounce is
    generic (
		width: integer := 22 -- 100 MHz clock -> 4 - simulation (90 ns); 22 - synthesis (ca. 20 ms)
	);
    port (
		clk : in STD_LOGIC;
        tl_in : in STD_LOGIC;
        tl_out : out STD_LOGIC
	);
end entity debounce;

architecture Behavioral of debounce is

signal cnt       : std_logic_vector(width-1 downto 0) := (others => '0');
signal reset, ce : std_logic := '0';
signal tl_prev   : std_logic := '0';


begin

    counter: process(clk) begin
        if rising_edge(clk) then 
            if (reset = '1') then
                cnt <= (others => '0');
            elsif (ce = '1') then
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    process(clk) begin
        if rising_edge(clk) then
            if (tl_prev /= tl_in) then
                reset <= '1';
            else 
                reset <= '0';
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if (reset = '1') then
                tl_prev <= tl_in;
            end if;
        end if;
    end process;
                
    process(clk) begin
        if rising_edge(clk) then
            if ((cnt(width-1) = '1') and (cnt(0) = '0')) then
                tl_out <= tl_prev;
            end if;
        end if;
    end process;
                

    ce <= '0' when ((cnt(width-1) = '1') and (cnt(0) = '1'))
                else '1';

end Behavioral;\n"

# ##############################################################
# Generates a VHDL instantiation template for the specified
# module.
# ##############################################################
proc vhdlTemplate { module } {
  global debounce_code
  global debounce_component

  set inPorts [lsort [get_bd_ports -quiet -filter DIR==I]]
  array set inputBusPorts [list ]
  set inputBitPorts [list]
  sortPorts $inPorts inputBusPorts inputBitPorts

  set outPorts [lsort [get_bd_ports -quiet -filter DIR==O]]
  array set outputBusPorts [list ]
  set outputBitPorts [list]
  sortPorts $outPorts outputBusPorts outputBitPorts

  set inoutPorts [lsort [get_bd_ports -quiet -filter DIR==IO]]
  array set inoutBusPorts [list ]
  set inoutBitPorts [list]
  sortPorts $inoutPorts inoutBusPorts inoutBitPorts

  set lines [list]
  # Process input single bit ports
#   lappend lines "-- Input Ports - Single Bit"
  foreach port [lsort -dictionary $inputBitPorts] {
    lappend lines [list "$port" "in  std_logic;"]
  }
  # Process input bus ports
#   lappend lines "-- Input Ports - Busses"
  foreach {port busInfo} [array2sortedList inputBusPorts] {
    lassign $busInfo stop start
    if {$start>$stop} {
      lappend lines [list "$port" "in  std_logic_vector($start downto $stop);"]
    } else {
      lappend lines [list "$port" "in  std_logic_vector($start to $stop);"]
    }
  }
  # Process output single bit ports
#   lappend lines "-- Output Ports - Single Bit"
  foreach port [lsort -dictionary $outputBitPorts] {
    lappend lines [list "$port" "out std_logic;"]
  }
  # Process output bus ports
#   lappend lines "-- Output Ports - Busses"
  foreach {port busInfo} [array2sortedList outputBusPorts] {
    lassign $busInfo stop start
    if {$start>$stop} {
      lappend lines [list "$port"  "out std_logic_vector($start downto $stop);"]
    } else {
      lappend lines [list "$port" "out std_logic_vector($start to $stop);"]
    }
  }
  # Process inout single bit ports
#   lappend lines "-- InOut Ports - Single Bit"
  foreach port [lsort -dictionary $inoutBitPorts] {
    lappend lines [list "$port" "inout  std_logic;"]
  }
  # Process inout bus ports
#   lappend lines "-- InOut Ports - Busses"
  foreach {port busInfo} [array2sortedList inoutBusPorts] {
    lassign $busInfo stop start
    if {$start>$stop} {
      lappend lines [list "$port" "inout  std_logic_vector($start downto $stop);"]
    } else {
      lappend lines [list "$port" "inout  std_logic_vector($start to $stop);"]
    }
  }

  # Detect maximum column width to align columns
  foreach line $lines {
    if {[regexp {^\s*\-\-} $line]} {
      # Skip lines that are just comments
      continue
    }
    set width [string length [lindex $line 0]]
    if {![info exist maxWidth] || $maxWidth < $width} {
      set maxWidth $width
    }
  }

  # Build the content of the stub:
  set content {}
  foreach line $lines {
    if {[regexp {^\s*\-\-} $line]} {
      # Lines that are just comments
      append content "\n      $line"
      continue
    }
    append content [format "\n      %-${maxWidth}s :  %-${maxWidth}s" [lindex $line 0] [lindex $line 1]]
  }

  # Remove the last semi-colon
  set index [string last {;} $content]
  set content [string replace $content $index $index {}]


  set nets [lsort [get_bd_nets -quiet]]
  array set busNets [list ]
  set bitNets [list]
  sortNets $nets busNets bitNets

  set lines [list]
  # Process single bit nets
  foreach net [lsort -dictionary $bitNets] {
    lappend lines [list "$net" "  std_logic;"]
  }
  # Process bus nets
  foreach {net busInfo} [array2sortedList busNets] {
    lassign $busInfo stop start
    if {$start>$stop} {
      lappend lines [list "$net" "  std_logic_vector($start downto $stop);"]
    } else {
      lappend lines [list "$net" "  std_logic_vector($start to $stop);"]
    }
  }

  # Detect maximum column width to align columns
  foreach line $lines {
    if {[regexp {^\s*\-\-} $line]} {
      # Skip lines that are just comments
      continue
    }
    set width [string length [lindex $line 0]]
    if {![info exist maxWidth] || $maxWidth < $width} {
      set maxWidth $width
    }
  }

  # Build the content of the stub:
  set signals {}
  foreach line $lines {
    if {[regexp {^\s*\-\-} $line]} {
      # Lines that are just comments
      append signals "\n      $line"
      continue
    }
    append signals [format "\n      signal %-${maxWidth}s :  %-${maxWidth}s" [lindex $line 0] [lindex $line 1]]
  }
  set ports2Signals {}
  foreach port [lsort -dictionary $inPorts] {
	set port_name [get_property NAME [get_bd_ports -quiet $port]]
	set net [get_bd_nets -quiet -of_objects [get_bd_ports -quiet $port]]
	if {$net != ""} {
		set net_name [get_property NAME $net]
		append ports2Signals "\t$net_name <= $port_name;\n"
	} else {
		catch {common::send_msg_id "bd2vhdl   port-2" "CRITICAL WARNING" "${port_name}: port nepripojen!"}
	}
  }
  foreach port [lsort -dictionary $outPorts] {
	set port_name [get_property NAME [get_bd_ports -quiet $port]]
	set net [get_bd_nets -quiet -of_objects [get_bd_ports -quiet $port]]
	if {$net != ""} {
		set net_name [get_property NAME $net]
		append ports2Signals "\t$port_name <= $net_name;\n"
	} else {
		catch {common::send_msg_id "bd2vhdl   port-3" "CRITICAL WARNING" "${port_name}: port nepripojen!"}
	}
  }
  set debounce false
  set components {}
  set modules {}
  set body [getIPs "" debounce components modules]
  
  if {$debounce} {
	  append components $debounce_component
  }
  # Now that the last semi-colon is replaced, add the header and footer
  set content [format "library IEEE;
use IEEE.std_logic_1164.all;

library xil_defaultlib;
use xil_defaultlib.all;

entity ${module}_vhdl is
   port (%s
   );
end entity ${module}_vhdl;

architecture Behavioral of ${module}_vhdl is
%s
%s
begin
%s
%s
end Behavioral;" $content $components $signals $ports2Signals $body]

  append content $modules

  if {$debounce} {
	  append content $debounce_code
  }

  return $content
}

set name [current_bd_design -quiet]
if {$name == ""} {
    common::send_msg_id -continue "bd2vhdl design-2" "ERROR" "Je potreba otevrit block design!"
	return
}
save_bd_design $name
validate_bd_design -force
if { [llength [get_bd_ports]] > 0 } {
	puts "\n##### ${name} #####"
	set content [vhdlTemplate $name]
	set PROJ_DIR [get_property DIRECTORY [current_project]]
	set PROJ_DIR [file normalize $PROJ_DIR]
	set tmp_dir [file join $PROJ_DIR "tmp"]
	file mkdir $tmp_dir
	set filename "${name}.vhd"
	set filename_path [file join $tmp_dir $filename]
	# Save the template
	set mode {w}
	puts "\nCreating VHDL for Design $name in [file normalize $filename]\n"
	set tfh [open $filename_path $mode]
	puts $tfh $content
	close $tfh

	import_files -force -norecurse $filename_path
	common::send_msg_id "bd2vhdl design-1" "INFO" "${name}_vhdl.vhd vygenerovan"
} else {
	common::send_msg_id -continue "bd2vhdl design-3" "ERROR" "${name} neobsahuje zadne porty!"
}