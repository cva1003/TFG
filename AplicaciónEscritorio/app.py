import json
import os
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.lang import Builder
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from openpyxl import load_workbook
import pandas as pd

usuarios_registrados = "usuarios.json"

def guardar_usuarios(usuarios):
    with open(usuarios_registrados, "w") as f:
        json.dump(usuarios, f)

def cargar_usuarios():
    if os.path.exists(usuarios_registrados):
        with open(usuarios_registrados, "r") as f:
            contenido = f.read()
            if contenido.strip():
                return json.loads(contenido)
    return {}

usuarios = cargar_usuarios()
def mostrar_advertencia(mensaje):
    popup = Popup(
        title='ERROR',
        content=Label(text=mensaje),
        size_hint=(None, None),
        size=(400, 200))
    popup.open()
class PrincipalWindow(Screen):
    pass
class PantallaPacienteScreen(Screen):
    def actualizar_nombre(self, nombre_completo):
        self.ids.paciente_nombre_input.text = f'Paciente: {nombre_completo.replace("_", " ") }'
class RegistroPacienteScreen(Screen):
    def registrar_datos_paciente(self):
        nombre = self.ids.paciente_nombre_input.text
        apellidos = self.ids.paciente_apellidos_input.text
        dia = self.ids.paciente_dia_input.text
        
        if not nombre or not apellidos or not dia:
            mostrar_advertencia("Es obligatorio rellenar todos los datos.")
        else: 
            app = App.get_running_app()
            app.root.current = "nueva"
        archivo_excel = "registro_pacientes.xlsx"
        nombre_hoja = f"{nombre}_{apellidos}".replace(" ", "_")

        if not os.path.exists(archivo_excel):
            with pd.ExcelWriter(archivo_excel, engine='openpyxl') as writer:
                df = pd.DataFrame(columns=["Fecha", "Indice", "Corazón","Anular","Meñique","Pulgar"]) 
                df.to_excel(writer, sheet_name=nombre_hoja, index=False)
        else:
            with pd.ExcelWriter(archivo_excel, engine='openpyxl', mode='a') as writer:
                book = load_workbook(archivo_excel)
                if nombre_hoja not in book.sheetnames:
                    df = pd.DataFrame(columns=["Fecha", "Indice", "Corazón","Anular","Meñique","Pulgar"])
                    df.to_excel(writer, sheet_name=nombre_hoja, index=False)
class NuevaScreen(Screen):
    def on_enter(self): 
        archivo = load_workbook("registro_pacientes.xlsx")
        pacientes = list(archivo.sheetnames)
        self.ids.paciente_spinner.values = pacientes
    def seleccionar_paciente(self, nombre_completo):
        if nombre_completo != "Seleccionar paciente":
            pantalla_paciente = self.manager.get_screen("PantallaPaciente")
            pantalla_paciente.actualizar_nombre(nombre_completo)
            self.manager.current = "PantallaPaciente"
class IniciarScreen(Screen):
    def verificar_login(self):
        usuario = self.ids.login_usuario.text
        contraseña = self.ids.login_contrasena.text
        if usuario in usuarios and usuarios[usuario]["contraseña"] == contraseña:
            app = App.get_running_app()
            app.nombre_usuario = usuario
            nueva_screen = app.root.get_screen("nueva")
            nueva_screen.ids.label_nombre.text = f" Profesional: {usuarios[usuario]['nombre']} {usuarios[usuario]['apellidos']} "
            app.root.current = "nueva"
        else:
            mostrar_advertencia("Usuario o contraseña incorrectos.")
class RegistroScreen(Screen):
    def registrar_usuario(self):
        nombre = self.ids.nombre_input.text
        apellidos = self.ids.apellidos_input.text
        usuario = self.ids.usuario_input.text
        contraseña = self.ids.contrasena_input.text
        if not usuario or not contraseña:
            mostrar_advertencia("Usuario y contraseña son obligatorios.")
        elif usuario in usuarios:
            mostrar_advertencia("El usuario ya está registrado.")
        else:
            usuarios[usuario] = {"nombre": nombre,"apellidos": apellidos,"contraseña": contraseña}
            guardar_usuarios(usuarios)
            App.get_running_app().root.current = "principal"
class MiApp(App):
    def build(self):
        return Builder.load_file("hospital.kv")
MiApp().run()
