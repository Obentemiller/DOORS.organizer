#<THE CODE>

import os
import time
import shutil
import tkinter as tk
from tkinter import filedialog


logo = """
  _____   ____   ____  _____   _____                             _              _            
 |  __ \ / __ \ / __ \|  __ \ / ____|                           (_)            | |           
 | |  | | |  | | |  | | |__) | (___   ___  _ __ __ _  __ _ _ __  _ ______ _  __| | ___  _ __ 
 | |  | | |  | | |  | |  _  / \___ \ / _ \| '__/ _` |/ _` | '_ \| |_  / _` |/ _` |/ _ \| '__|
 | |__| | |__| | |__| | | \ \ ____) | (_) | | | (_| | (_| | | | | |/ / (_| | (_| | (_) | |   
 |_____/ \____/ \____/|_|  \_\_____(_)___/|_|  \__, |\__,_|_| |_|_/___\__,_|\__,_|\___/|_|   
                                                __/ |                                        
                                               |___/                                         
     github - Obentemiller V(0.1)
"""

def exibir_ascii_lista_com_atraso(lista_ascii, atraso=1):
    for ascii_art in lista_ascii:
        print(ascii_art)
        time.sleep(atraso)
        os.system('cls' if os.name == 'nt' else 'clear')

ascii_art0 = '''
    ._________________.       
    |.---------------.|       
    ||               ||       
    ||    PRONTO!    ||        
    ||               ||       
    ||               ||       
    ||_______________||       
    /.-.-.-.-.-.-.-.-.\       
   /.-.-.-.-.-.-.-.-.-.\      
  /.-.-.-.-.-.-.-.-.-.-.\     
 /______/__________\___o_\\
 \_______________________/                                                             
'''

ascii_art1 = '''

    ._________________.       
    |.---------------.|       
    ||               ||       
    ||   ATÉ MAIS!   ||        
    ||  Obentemiller ||       
    ||               ||       
    ||_______________||       
    /.-.-.-.-.-.-.-.-.\\       
   /.-.-.-.-.-.-.-.-.-.\\      
  /.-.-.-.-.-.-.-.-.-.-.\\     
 /______/__________\___o_\\
 \_______________________/                                                             
'''

ascii_art2 = ''' 





     _________________     
    /                 \\      
   /                   \\       
  /                     \\      
 /_____________obrigado _\\     
 |-----------------------|
 \_______________________/                                                             
'''
lista_ascii = [ascii_art0, ascii_art1, ascii_art2]

def escolher_pasta():
    root = tk.Tk()
    root.withdraw() 

    pasta = filedialog.askdirectory(title="Escolha a pasta a ser organizada")
    return pasta

def organizar_por_categoria(pasta):
    for arquivo in os.listdir(pasta):
        caminho_arquivo = os.path.join(pasta, arquivo)
        
        if os.path.isfile(caminho_arquivo):
            formato = os.path.splitext(arquivo)[1].lower()

            if formato:
                categoria = obter_categoria(formato)
                destino = os.path.join(pasta, categoria)
                os.makedirs(destino, exist_ok=True)
                shutil.move(caminho_arquivo, os.path.join(destino, arquivo))

    print("Organização concluída!")
    exibir_ascii_lista_com_atraso(lista_ascii)

def obter_categoria(formato):
    documentos = {'.pdf', '.doc', '.docx', '.txt', '.ppt', '.pptx', '.xls', '.xlsx', '.odt', '.odp', '.ods'}
    imagens = {'.png', '.jpg', '.jpeg', '.gif', '.bmp', '.tiff'}
    videos = {'.mp4', '.avi', '.mov', '.mkv', '.wmv', '.flv'}
    atalhosandexe = {'.lnk','.exe'}

    formato_lower = formato.lower()

    if formato_lower in documentos:
        if formato_lower in {'.odt', '.odp', '.ods'}:
            return 'Documentos/LibreOffice'
        elif formato_lower in {'.doc', '.docx', '.ppt', '.pptx', '.xls', '.xlsx'}:
            return 'Documentos/PacoteOffice'
        else:
            return 'Documentos/Outros'
    elif formato_lower in imagens:
        return 'Imagens'
    elif formato_lower in videos:
        return 'Videos'
    elif formato.lower() in atalhosandexe:
        return 'atalhos and exe'
    else:
        return 'Outros'


def main():
    print(logo)
    pasta_a_organizar = escolher_pasta()

    if not pasta_a_organizar:
        print("Nenhuma pasta selecionada. Saindo...")
        return

    print(f"Pasta selecionada!: {'<'}{pasta_a_organizar}{'>'}")
    organizar_por_categoria(pasta_a_organizar)

if __name__ == "__main__":
    main()

