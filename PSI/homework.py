import os
import socket
from time import sleep

# Create socket ip/tcp
l = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Bind the socket to *this* device and port 6666
l.bind(('localhost', 7017))
# Maximum amount of clients, listening
l.listen(10)

# Authentication keys [](server key, client key):
auth_keys = [(23019, 32037), (32037, 29295), (18789, 13603), (16443, 29533), (18189, 21952)]

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
        # ----- Authentication ----- #
        # 1) Wait for CLIENT_USERNAME
        username = c.recv(20).decode()
        username = username.replace('\a\b', '')
        # 2) Ask robot to send the key
        c.send("107 KEY REQUEST\a\b".encode())
        # 3) Wait for CLIENT_KEY_ID
        key_id = c.recv(5).decode()
        key_id = key_id.replace('\a\b', '')
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
        confirm_key = c.recv(12).decode()
        confirm_key = confirm_key.replace('\a\b', '')
        # 7) Send SERVER_OK/LOGIN_FAILED
        if (int(confirm_key) - auth_keys[int(key_id)][1]) % 65536 == orig_code:
            c.send("200 OK\a\b".encode())
        else:
            c.send("300 LOGIN FAILED\a\b".encode())
            c.close()
            l.close()
            break
        # -------------------------- #
        # ------ Moving robot ------ #
        # Robot can be in 4 states:
        # 1. Facing up
        # 2. Facing right
        # 3. Facing down
        # 4. Facing left
        states = [(0, "Up"), (1, "Right"), (2, "Down"), (3, "Left")]
        # To find out robots state, server moves it twice
        state = 0
        c.send("102 MOVE\a\b".encode())
        coord_message = c.recv(12).decode()
        coord_message = coord_message.replace('\a\b', '')
        coords = coord_message.split()
        prev_x = coords[1]
        prev_y = coords[2]
        print(f"x = {prev_x}, y = {prev_y}")
        c.send("102 MOVE\a\b".encode())
        coord_message = c.recv(12).decode()
        coord_message = coord_message.replace('\a\b', '')
        print(coord_message)
        coords = coord_message.split()
        print(coords)
        coord_x = coords[1]
        coord_y = coords[2]
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
        if int(coord_y) > 0:
            while state != 2:
                c.send("104 TURN RIGHT\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"8Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
                state += 1
                state %= 4
            while coord_y != 0:
                c.send("102 MOVE\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"7Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        else:
            while state != 0:
                c.send("104 TURN RIGHT\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"6Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
                state += 1
                state %= 4
            while int(coord_y) != 0:
                c.send("102 MOVE\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"5Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        # Move to the start of the coordinates
        if int(coord_x) < 0:
            while state != 1:
                c.send("104 TURN RIGHT\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"4Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
                state += 1
                state %= 4
            while int(coord_x) != 0:
                c.send("102 MOVE\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"3Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        else:
            while state != 3:
                c.send("104 TURN RIGHT\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"2Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
                state += 1
                state %= 4
            while int(coord_x) != 0:
                c.send("102 MOVE\a\b".encode())
                coord_message = c.recv(12).decode()
                coord_message = coord_message.replace('\a\b', '')
                coords = coord_message.split()
                coord_x = coords[1]
                coord_y = coords[2]
                print(f"1Facing {states[state][1]}, x = {coord_x}, y = {coord_y}")
        print(f"I should be in 0, 0: x = {coord_x}, y = {coord_y}")

        # -------------------------- #
        # - Pick up secret message - #
        c.send("105 GET MESSAGE\a\b".encode())
        secret_message = c.recv(100).decode()
        secret_message = secret_message.replace('\a\b', '')
        print(f"Found: '{secret_message}'")
        c.send("106 LOGOUT\a\b".encode())


        # c.close()
        # l.close()
        # break #child executes only one cycle

    except socket.timeout as e: #if timeout occurs
        print("Timeout!")
        c.close()
        l.close()

    finally:
        c.close()
        l.close()
        break


