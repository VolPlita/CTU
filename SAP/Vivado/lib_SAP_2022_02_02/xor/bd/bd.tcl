# soubor se nesmi presouvat do jineho adresare
# pri zmene nutno restartovat Vivado

proc init {cellpath otherInfo} {
#  puts "init xor v4"
}

proc post_config_ip_set {pin value} {
#    puts $pin
#    puts $value
    if {$value} {
      set_property CONFIG.POLARITY "ACTIVE_LOW" $pin
    } else {
      set_property CONFIG.POLARITY "ACTIVE_HIGH" $pin
    }
}



proc post_config_ip {cellName otherInfo} {
#    puts "post_config_ip xor v6"
    set cell [get_bd_cells $cellName]
    set inputs [get_property CONFIG.inputs $cell]
      
    set pin [get_bd_pins ${cellName}/I0]
    set value [get_property CONFIG.I0_Inverted $cell]
    post_config_ip_set $pin $value

    set pin [get_bd_pins ${cellName}/I1]
    set value [get_property CONFIG.I1_Inverted $cell]
    post_config_ip_set $pin $value

    if {$inputs > 2} {
      set pin [get_bd_pins ${cellName}/I2]
      set value [get_property CONFIG.I2_Inverted $cell]
      post_config_ip_set $pin $value
    
    if {$inputs > 3} {
      set pin [get_bd_pins ${cellName}/I3]
      set value [get_property CONFIG.I3_Inverted $cell]
      post_config_ip_set $pin $value

    if {$inputs > 4} {
      set pin [get_bd_pins ${cellName}/I4]
      set value [get_property CONFIG.I4_Inverted $cell]
      post_config_ip_set $pin $value

    if {$inputs > 5} {
      set pin [get_bd_pins ${cellName}/I5]
      set value [get_property CONFIG.I5_Inverted $cell]
      post_config_ip_set $pin $value

    if {$inputs > 6} {
      set pin [get_bd_pins ${cellName}/I6]
      set value [get_property CONFIG.I6_Inverted $cell]
      post_config_ip_set $pin $value

    if {$inputs > 7} {
      set pin [get_bd_pins ${cellName}/I7]
      set value [get_property CONFIG.I7_Inverted $cell]
      post_config_ip_set $pin $value
    }}}}}}

    set pin [get_bd_pins ${cellName}/O]
    set value [get_property CONFIG.O_Inverted $cell]
    post_config_ip_set $pin $value
}

proc propagate {cellName otherInfo} {
}

proc post_propagate_set {config cell pin} {
    set polarity [get_property CONFIG.POLARITY [get_bd_pins $pin]]

#    puts $config
#    puts $cell
#    puts $pin
#    puts $polarity

    if {$polarity == "ACTIVE_LOW"} {
      set_property $config true $cell
    }
    if {$polarity == "ACTIVE_HIGH"} {
      set_property $config false $cell
    }
}

proc post_propagate {cellName otherInfo} {
#    puts "post_propagate xor v6"

    set cell [get_bd_cells $cellName]
    set inputs [get_property CONFIG.inputs $cell]

      
    set pin [get_bd_pins ${cellName}/I0]    
    set config [get_property CONFIG.I0_Inverted $cell]
    post_propagate_set $config $cell $pin

    set pin [get_bd_pins ${cellName}/I1]    
    set config [get_property CONFIG.I1_Inverted $cell]
    post_propagate_set $config $cell $pin

    if {$inputs > 2} {
      set pin [get_bd_pins ${cellName}/I2]    
      set config [get_property CONFIG.I2_Inverted $cell]
      post_propagate_set $config $cell $pin

    if {$inputs > 3} {
      set pin [get_bd_pins ${cellName}/I3]    
      set config [get_property CONFIG.I3_Inverted $cell]
      post_propagate_set $config $cell $pin

    if {$inputs > 4} {
      set pin [get_bd_pins ${cellName}/I4]    
      set config [get_property CONFIG.I4_Inverted $cell]
      post_propagate_set $config $cell $pin

    if {$inputs > 5} {
      set pin [get_bd_pins ${cellName}/I5]    
      set config [get_property CONFIG.I5_Inverted $cell]
      post_propagate_set $config $cell $pin

    if {$inputs > 6} {
      set pin [get_bd_pins ${cellName}/I6]    
      set config [get_property CONFIG.I6_Inverted $cell]
      post_propagate_set $config $cell $pin

    if {$inputs > 7} {
      set pin [get_bd_pins ${cellName}/I7]    
      set config [get_property CONFIG.I7_Inverted $cell]
      post_propagate_set $config $cell $pin
    }}}}}}

    set pin [get_bd_pins ${cellName}/O]    
    set config [get_property CONFIG.O_Inverted $cell]
    post_propagate_set $config $cell $pin

}
