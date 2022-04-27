

def create_config_dot_h():
    with open(".config") as dot_config_file:
        config_dot_h_file_content = ""
        
        def is_commented(line):
            if '#' in line:
                return True
            return False
        
        def is_valid_config(line):
            if '=' in line:
                return True
            return False

        def create_define(name, value):
            if value == "y":
                value = 1
            return "#define %s %s\n" %(name, value)
        
        for line in dot_config_file.readlines():
            if not is_commented(line) and is_valid_config(line):
                [name, raw_value] = line.split("=")
                raw_value = raw_value[:-1]
                config_dot_h_file_content += create_define(name, raw_value)
    
        print(config_dot_h_file_content)
        
        with open("config.h", 'w') as config_dot_h:
            config_dot_h.write(config_dot_h_file_content)
            config_dot_h.close()
    
create_config_dot_h()