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
        # c.settimeout(1)
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
        # print(self.messages_arr)
        return self.messages_arr.pop(0)


# Create message receiver
receiver = Receiver()


def main():
    # Create socket ip/tcp
    l = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Bind the socket to *this* device and port 6666
    l.bind(('localhost', 7061))
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
            state, coord_x, coord_y = s_move_to_x(c, states, x_bypass_left, x_bypass_right)
            # Move robot to the start of coordinates along X-axis
            coord_x, coord_y = s_move_to_start(c, states, state, coord_x, coord_y)
            # Check if on the correct position
            if int(coord_y) != 0:
                state, coord_x, coord_y = s_move_to_x(c, states, down_bypass, up_bypass)
            print(f"State: {state}, X: {coord_x}, Y: {coord_y}")

            # - Pick up secret message - #
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


# def check_coord_message(coord_message):
#     if :
#
#     return coord_x, coord_y


def s_bypass_left(c, states, state):
    print("S Bypass l")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # Check length?
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def s_bypass_right(c, states, state):
    print("S Bypass l")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def s_move_to_start(c, states, state, coord_x, coord_y):
    if int(coord_x) < 0:
        while state != 1:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            coord_x = coords[1]
            coord_y = coords[2]
            print(f"4Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_x) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            prev_x = coord_x
            prev_y = coord_y
            coord_x = coords[1]
            coord_y = coords[2]
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                s_bypass_left(c, states, state)
            print(f"3  Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    else:
        while state != 3:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            coord_x = coords[1]
            coord_y = coords[2]
            print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_x) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            prev_x = coord_x
            prev_y = coord_y
            coord_x = coords[1]
            coord_y = coords[2]
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                s_bypass_right(c, states, state)
            print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    print(f"I should be in 0, 0: x = {coord_x}, y = {coord_y}")
    return coord_x, coord_y


def receive_coord_answer():
    print("TODO")


def x_bypass_left(c, states, state):
    print("Bypass left x")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1xFacing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def x_bypass_right(c, states, state):
    print("Bypass right")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def down_bypass(c, states, state):
    # We need to look right
    while state != 1:
        c.send("104 TURN RIGHT\a\b".encode())
        coord_message = receiver.get_next_message(c)
        # coord_message = c.recv(12).decode()
        # coord_message = coord_message.replace('\a\b', '')
        coords = coord_message.split()
        coord_x = coords[1]
        coord_y = coords[2]
        print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        state += 1
        state %= 4
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def up_bypass(c, states, state):
    # We need to look right
    while state != 1:
        c.send("104 TURN RIGHT\a\b".encode())
        coord_message = receiver.get_next_message(c)
        # coord_message = c.recv(12).decode()
        # coord_message = coord_message.replace('\a\b', '')
        coords = coord_message.split()
        coord_x = coords[1]
        coord_y = coords[2]
        print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        state += 1
        state %= 4
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("103 TURN LEFT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("104 TURN RIGHT\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")


def s_move_to_x(c, states, left_bypass, right_bypass):
    # To find out robots state, server moves it twice
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    coords = coord_message.split()
    prev_x = coords[1]
    prev_y = coords[2]
    print(f"x = {prev_x}, y = {prev_y}")
    c.send("102 MOVE\a\b".encode())
    coord_message = receiver.get_next_message(c)
    # coord_message = c.recv(12).decode()
    # coord_message = coord_message.replace('\a\b', '')
    print(coord_message)
    coords = coord_message.split()
    print(coords)
    coord_x = coords[1]
    coord_y = coords[2]
    print(f"x = {coord_x}, y = {coord_y}")
    # If spawned right before an obstacle
    if prev_x == coord_x and prev_y == coord_y:
        c.send("103 TURN LEFT\a\b".encode())
        coord_message = receiver.get_next_message(c)
        coords = coord_message.split()
        coord_x = coords[1]
        coord_y = coords[2]
        print(f"x = {coord_x}, y = {coord_y}")
        c.send("102 MOVE\a\b".encode())
        coord_message = receiver.get_next_message(c)
        coords = coord_message.split()
        coord_x = coords[1]
        coord_y = coords[2]
        print(f"x = {coord_x}, y = {coord_y}")
    if prev_x == coord_x:
        if int(coord_y) > int(prev_y):
            state = 0
        else:
            state = 2
    else:
        if int(coord_x) > int(prev_x):
            state = 1
        else:
            state = 3
    print(f"Facing {states[state][1]}, {states[state][0]}")
    # Navigate towards the x-axis
    if int(coord_y) > 0:
        while state != 2:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            coord_x = coords[1]
            coord_y = coords[2]
            print(f"8Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_y) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            prev_x = coord_x
            prev_y = coord_y
            coord_x = coords[1]
            coord_y = coords[2]
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                if int(coord_x) < 0:
                    left_bypass(c, states, state)
                else:
                    right_bypass(c, states, state)
            print(f"7Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    else:
        while state != 0:
            c.send("104 TURN RIGHT\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            coord_x = coords[1]
            coord_y = coords[2]
            print(f"6Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
            state += 1
            state %= 4
        while int(coord_y) != 0:
            c.send("102 MOVE\a\b".encode())
            coord_message = receiver.get_next_message(c)
            # coord_message = c.recv(12).decode()
            # coord_message = coord_message.replace('\a\b', '')
            coords = coord_message.split()
            prev_x = coord_x
            prev_y = coord_y
            coord_x = coords[1]
            coord_y = coords[2]
            if int(coord_x) == int(prev_x) and int(coord_y) == int(prev_y):
                if int(coord_x) > 0:
                    left_bypass(c, states, state)
                else:
                    right_bypass(c, states, state)
            print(f"5Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
    return state, coord_x, coord_y


def s_authentication(c):
    # Authentication keys [](server key, client key):
    auth_keys = [(23019, 32037), (32037, 29295), (18789, 13603), (16443, 29533), (18189, 21952)]

    # 1) Wait for CLIENT_USERNAME
    username = receiver.get_next_message(c)
    # print(username)
    if len(username) > 18:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    # username = c.recv(20).decode()
    # username = username.replace('\a\b', '')
    if "\a\b" in username:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    # 2) Ask robot to send the key
    c.send("107 KEY REQUEST\a\b".encode())
    # 3) Wait for CLIENT_KEY_ID
    key_id = receiver.get_next_message(c)
    print(key_id)
    # key_id = c.recv(5).decode()
    # key_id = key_id.replace('\a\b', '')
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
    if ' ' in confirm_key or len(confirm_key) > 5:
        c.send("301 SYNTAX ERROR\a\b".encode())
        return False
    # confirm_key = c.recv(12).decode()
    # confirm_key = confirm_key.replace('\a\b', '')
    # 7) Send SERVER_OK/LOGIN_FAILED
    if (int(confirm_key) - auth_keys[int(key_id)][1]) % 65536 == orig_code:
        c.send("200 OK\a\b".encode())
        return True
    else:
        c.send("300 LOGIN FAILED\a\b".encode())
        return False


if __name__ == '__main__':
    main()
