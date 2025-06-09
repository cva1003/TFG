from cryptography.fernet import Fernet

def cargar_clave():
    return open('clave.key', 'rb').read()

def encriptar(nom_archivo, clave):
    f = Fernet(clave)
    with open(nom_archivo, 'rb') as file:
        archivo_info = file.read()
    encrypted_data = f.encrypt(archivo_info)
    with open(nom_archivo, 'wb') as file:
        file.write(encrypted_data)

def desencriptar(nom_archivo, clave):
    f = Fernet(clave)
    with open(nom_archivo, 'rb') as file:
        encrypted_data = file.read()
    decrypted_data = f.decrypt(encrypted_data)
    with open(nom_archivo, 'wb') as file:
        file.write(decrypted_data)

clave = cargar_clave()

encriptar("registro_pacientes.xlsx", clave)
encriptar("usuarios.json", clave)
