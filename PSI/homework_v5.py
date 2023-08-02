import os
import socket
from time import sleep


class Receiver:
    def __init__(self):
        self.messages_arr = []
        self.current_string = ""
        self.complete = False

    def free(self):
        self.messages_arr = []
        self.current_string = ""
        self.complete = False

    def get_next_message(self, c):
        while not self.complete:
            if len(self.messages_arr) == 0:
                self.current_string = c.recv(1024).decode()
                if self.current_string[-2:] != "\a\b":
                    self.complete = False
                else:
                    self.complete = True
                self.messages_arr = self.current_string.split("\a\b")
            elif len(self.messages_arr) == 1 and not self.complete:  # A little redundant if
                self.current_string = ""
                self.current_string = self.messages_arr.pop(0)
                self.current_string += c.recv(1024).decode()
                if self.current_string[-2:] != "\a\b":
                    self.complete = False
                else:
                    self.complete = True
                self.messages_arr = []  # A little redundant
                self.messages_arr = self.current_string.split("\a\b")
            else:
                break
        self.complete = False
        return self.messages_arr.pop(0)


# Create message receiver
receiver = Receiver()


def main():
    # Create socket ip/tcp
    l = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Bind the socket to *this* device and port 6666
    l.bind(('localhost', 7063))
    # Maximum amount of clients, listening
    l.listen(10)

    # Infinite loop that waits for connection from robot
    while True:
        # Accept returns newly created socket and address of the client
        c, address = l.accept()
        # Fork returns process id of the child - stored in the parent
        child_pid = os.fork()
        # Close communication socket for parent thread
        if child_pid != 0:
            c.close()
            continue
        # Close listening thread for child
        l.close()

        # Set timeout
        c.settimeout(1)
        try:
            # Authentication step
            if not s_authentication(c):
                c.close()
                l.close()
                break

            # Robot can be in 4 states:
            # 1. Facing up - 0
            # 2. Facing right - 1
            # 3. Facing down - 2
            # 4. Facing left - 3
            states = [(0, "Up"), (1, "Right"), (2, "Down"), (3, "Left")]

            # Move robot to the X-axis
            ok, state, coord_x, coord_y = s_move_to_x(c, states)
            if not ok:
                break
            # Move robot to the start of coordinates along X-axis
            ok, coord_x, coord_y = s_move_to_start(c, states, state, coord_x, coord_y)
            if not ok:
                break

            # - Pick up secret message - #
            if coord_x == 0 and coord_y == 0:
                c.send("105 GET MESSAGE\a\b".encode())
                secret_message = receiver.get_next_message(c)
                if len(secret_message) > 98:
                    c.send("302 LOGIC ERROR\a\b".encode())
                    break
                print(f"Found: '{secret_message}'")
            c.send("106 LOGOUT\a\b".encode())
            receiver.free()

        except socket.timeout as e:  # if timeout occurs
            print("Timeout!")
            c.close()
            l.close()
            receiver.free()

        finally:
            c.close()
            l.close()
            receiver.free()
            break


def check_coord_message(coord_message, c):
    if coord_message == "RECHARGING":
        c.settimeout(5)
        if receiver.get_next_message(c) != "FULL POWER":
            return False
        c.settimeout(1)
        coord_message = receiver.get_next_message(c)
    coord_arr = coord_message.split(' ')
    if coord_message[-1] == ' ' or len(coord_arr) != 3 or coord_arr[0] != "OK" or '.' in coord_arr[1] or '.' in coord_arr[2]:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    else:
        return True


def return_coord_message(coord_message):
    coord_arr = coord_message.split(' ')
    coord_x = int(coord_arr[1])
    coord_y = int(coord_arr[2])
    return coord_x, coord_y


def s_bypass_left(c, states, state, coord_x, coord_y):
    print("S Bypass l")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    return prev_x, prev_y, coord_x, coord_y


def s_bypass_right(c, states, state, coord_x, coord_y):
    print("S Bypass r")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    return prev_x, prev_y, coord_x, coord_y


def s_move_to_start(c, states, state, coord_x, coord_y):
    if int(coord_x) < 0:
        while state != 1:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            print(f"4Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_x) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                prev_x, prev_y, coord_x, coord_y = s_bypass_left(c, states, state, coord_x, coord_y)
                if coord_x == 0:
                    break
            print(f"3  Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    else:
        while state != 3:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_x) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            if coord_x == prev_x and coord_y == prev_y:
                prev_x, prev_y, coord_x, coord_y = s_bypass_right(c, states, state, coord_x, coord_y)
                if coord_x == 0:
                    break
            print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    print(f"I should be in 0, 0: x = {coord_x}, y = {coord_y}")
    return True, coord_x, coord_y


def x_bypass_left(c, states, state, coord_x, coord_y):
    print("Bypass left x")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def x_bypass_right(c, states, state, coord_x, coord_y):
    print("Bypass right")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = coord_x, coord_y
    coord_x, coord_y = return_coord_message(coord_message)
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def s_move_to_x(c, states):
    # To find out robots state, server moves it twice
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    prev_x, prev_y = return_coord_message(coord_message)
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    if not check_coord_message(coord_message, c):
        return False
    coord_x, coord_y = return_coord_message(coord_message)
    # If spawned right before an obstacle
    if prev_x == coord_x and prev_y == coord_y:
        c.send("103 TURN LEFT\a\b".encode())
        coord_message = receiver.get_next_message(c)
        if not check_coord_message(coord_message, c):
            return False
        prev_x, prev_y = coord_x, coord_y
        coord_x, coord_y = return_coord_message(coord_message)
        print(f"x = {coord_x}, y = {coord_y}")
        c.send("102 MOVE\a\b".encode())
        coord_message = receiver.get_next_message(c)
        if not check_coord_message(coord_message, c):
            return False
        prev_x, prev_y = coord_x, coord_y
        coord_x, coord_y = return_coord_message(coord_message)
        print(f"x = {coord_x}, y = {coord_y}")
    if prev_x == coord_x:
        if coord_y > prev_y:
            state = 0
        else:
            state = 2
    else:
        if coord_x > prev_x:
            state = 1
        else:
            state = 3
    print(f"Facing {states[state][1]}, {states[state][0]}")
    # Navigate towards the x-axis
    if coord_y > 0:
        while state != 2:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            print(f"8Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while coord_y != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                if int(coord_x) < 0:
                    x_bypass_left(c, states, state, coord_x, coord_y)
                else:
                    x_bypass_right(c, states, state, coord_x, coord_y)
            print(f"7Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    else:
        while state != 0:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            print(f"6Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while coord_y != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            if not check_coord_message(coord_message, c):
                return False
            prev_x, prev_y = coord_x, coord_y
            coord_x, coord_y = return_coord_message(coord_message)
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                if int(coord_x) > 0:
                    x_bypass_left(c, states, state, coord_x, coord_y)
                else:
                    x_bypass_right(c, states, state, coord_x, coord_y)
            print(f"5Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    return True, state, coord_x, coord_y


def s_authentication(c):
    # Authentication keys [](server key, client key):
    auth_keys = [(23019, 32037), (32037, 29295), (18789, 13603), (16443, 29533), (18189, 21952)]

    # 1) Wait for CLIENT_USERNAME
    username = receiver.get_next_message(c)
    if len(username) > 18:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    if "\a\b" in username:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    # 2) Ask robot to send the key
    c.send("107 KEY REQUEST\a\b".encode())
    # 3) Wait for CLIENT_KEY_ID
    key_id = receiver.get_next_message(c)
    if key_id == "RECHARGING":
        c.settimeout(5)
        if receiver.get_next_message(c) != "FULL POWER":
            return False
        c.settimeout(1)
        key_id = receiver.get_next_message(c)
    if not key_id.isdigit():
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    elif int(key_id) < 0 or int(key_id) > 4:
        c.send("303 KEY OUT OF RANGE\a\b".encode())
        return False
    # 4) Calculate hash code
    hash_code = 0
    for char in username:
        hash_code += ord(char)
    hash_code *= 1000
    hash_code %= 65536
    orig_code = hash_code
    hash_code += auth_keys[int(key_id)][0]
    hash_code %= 65536
    # 5) Send server confirmation
    str_code = str(hash_code) + '\a\b'
    c.send(str_code.encode())
    # 6) Receive client confirmation
    confirm_key = receiver.get_next_message(c)
    if confirm_key == "RECHARGING":
        c.settimeout(5)
        if receiver.get_next_message(c) != "FULL POWER":
            return False
        confirm_key = receiver.get_next_message(c)
    if ' ' in confirm_key or len(confirm_key) > 5:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    # 7) Send SERVER_OK/LOGIN_FAILED
    if (int(confirm_key) - auth_keys[int(key_id)][1]) % 65536 == orig_code:
        c.send("200 OK\a\b".encode())
        return True
    else:
        c.send("300 LOGIN FAILED\a\b".encode())
        return False


if __name__ == '__main__':
    main()
