import cv2
import numpy as np
import urllib.request
import main  

# Parámetros de la parábola
altura_inicial = 1.2
g = 9.81
velocidad_inicial = 40

# Parámetros de la cámara
res_x = 480
res_y = 320
f = 800
cx = res_x // 2
cy = res_y // 2

matriz_camara = np.array([[f, 0, cx], [0, f, cy], [0, 0, 1]], dtype=np.float32)
coef_distorsion = np.zeros((4, 1))

# Transformación 3D
vector_rotacion = np.array([[np.radians(-90)], [np.radians(4)], [np.radians(-8)]], dtype=np.float32)
vector_traslacion = np.array([[-18], [-3.5], [-69]], dtype=np.float32)

# URL de la cámara (JPEG)
url = "http://192.168.152.187/400x296.jpg"

def calcular_parabola(angulo_deg):
    angulo_rad = np.radians(angulo_deg)
    distancia_max = (velocidad_inicial**2 * np.sin(2 * angulo_rad)) / g
    y = np.linspace(0, distancia_max, 8000)
    z = altura_inicial + y * np.tan(angulo_rad) - ((g * y ** 2) / (2 * velocidad_inicial ** 2 * np.cos(angulo_rad) ** 2))
    x = np.zeros_like(y)
    puntos_3D = np.stack((x, y, z), axis=1).astype(np.float32)
    puntos_2D, _ = cv2.projectPoints(puntos_3D, vector_rotacion, vector_traslacion, matriz_camara, coef_distorsion)
    return puntos_2D.reshape(-1, 2)

while True:
    try:
        # Leer imagen desde la ESP32-CAM
        respuesta = urllib.request.urlopen(url)
        datos = bytearray(respuesta.read())
        imagen_np = np.array(datos, dtype=np.uint8)
        frame = cv2.imdecode(imagen_np, cv2.IMREAD_COLOR)

        if frame is None:
            continue

        frame = cv2.resize(frame, (res_x, res_y))

        # Dibujar cruz en el centro de la imagen
        centro_x = res_x // 2
        centro_y = res_y // 2
        tam_linea = 10  # longitud de la cruz

        # Línea horizontal
        cv2.line(frame, (centro_x - tam_linea, centro_y), (centro_x + tam_linea, centro_y), (0, 255, 0), 1)

        # Línea vertical
        cv2.line(frame, (centro_x, centro_y - tam_linea), (centro_x, centro_y + tam_linea), (0, 255, 0), 1)


        # Obtener ángulo actual desde el servidor Flask
        angulo_deg = main.get_angulo_actual()
        puntos_2D = calcular_parabola(angulo_deg)

        # Dibujar la parábola
        for pt in puntos_2D:
            x, y = int(pt[0]), int(pt[1])
            if 0 <= x < res_x and 0 <= y < res_y:
                cv2.circle(frame, (x, y), 2, (0, 0, 255), -1)

        cv2.imshow("Super Tanque #01", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    except Exception as e:
        print("Error:", e)
        cv2.waitKey(100)

cv2.destroyAllWindows()
