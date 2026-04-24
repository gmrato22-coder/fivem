import os

# Convierte una imagen PNG a un array binario hexadecimal para incluir en código C++

def convertir_a_array_binario(imagen_ruta, nombre_array, carpeta_destino):
    # Abre la imagen en modo binario
    with open(imagen_ruta, "rb") as archivo_imagen:
        # Lee todo el contenido del archivo
        contenido = archivo_imagen.read()
    
    # Verifica que la carpeta destino exista, si no, la crea
    if not os.path.exists(carpeta_destino):
        os.makedirs(carpeta_destino)
    
    # Define la ruta completa del archivo de salida
    ruta_salida = os.path.join(carpeta_destino, f"{nombre_array}.hpp")
    
    # Abre el archivo .hpp en modo escritura
    with open(ruta_salida, "w") as archivo_hpp:
        archivo_hpp.write(f"// Array binario de la imagen {imagen_ruta}\n")
        archivo_hpp.write(f"unsigned char {nombre_array}[] = {{\n")
        
        # Escribe cada byte del archivo como hexadecimal
        for i, byte in enumerate(contenido):
            if i % 12 == 0 and i != 0:  # Organiza el código para que sea más legible (12 bytes por línea)
                archivo_hpp.write("\n")
            archivo_hpp.write(f"0x{byte:02X}, ")

        archivo_hpp.write("\n};\n")

    print(f"Archivo {nombre_array}.hpp generado correctamente en {ruta_salida}.")

# Ruta de la imagen (ajusta esta ruta a la ubicación de tu imagen)
imagen_ruta = r"C:\Users\gabri\Escritorio\APRENDER A PROGRAMAR\LEAKS GG\Wexize-V2-main (1) (1)\Wexize-V2-main\Sin título-2.png"

# Nombre del array para el código binario
nombre_array = "logo_binary"

# Carpeta donde quieres guardar el archivo .hpp (ajusta la ruta de la carpeta)
carpeta_destino = r"C:\Users\gabri\Escritorio\APRENDER A PROGRAMAR\IMAGEN BIN"

# Llama a la función con los parámetros adecuados
convertir_a_array_binario(imagen_ruta, nombre_array, carpeta_destino)

