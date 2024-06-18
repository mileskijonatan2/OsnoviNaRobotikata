import serial
import time
import matplotlib.pyplot as plt

if __name__ == '__main__':

    COM_PORT = 'COM5'
    BAUD_RATE = 9600

    x_coordinates, y_coordinates, z_coordinates = [], [], []

    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE)
        time.sleep(2)

        with open('output.txt', 'w') as file:
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
                    if "END" == line:
                        ser.close()
                        break
                    x, y, z, ang, org_pt = line.split(", ")
                    x_coordinates.append(float(x))
                    y_coordinates.append(float(y))
                    z_coordinates.append(float(z))
                    if float(ang) >= 112:
                        ser.close()
                        break
                    file.write(f'({x}, {y}, {z})' + '\n')

    except serial.SerialException as e:
        print(f"Error: Could not open port {COM_PORT}. {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

    '''plt.scatter(x_coordinates, y_coordinates)
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('2D Scatter Plot')
    plt.show()'''

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    scatter = ax.scatter(x_coordinates, y_coordinates, z_coordinates, c=z_coordinates, cmap='viridis')

    color_bar = fig.colorbar(scatter, ax=ax)

    ax.set_xlabel('X Label')
    ax.set_ylabel('Y Label')
    ax.set_zlabel('Z Label')

    plt.show()

