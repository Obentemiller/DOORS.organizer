import os
import time
import shutil
import tkinter as tk
from tkinter import filedialog
from transformers import AutoModel, AutoTokenizer
import torch
from sklearn.metrics.pairwise import cosine_similarity

logo = """
  _____   ____   ____  _____   _____                             _              _            
 |  __ \ / __ \ / __ \|  __ \ / ____|                           (_)            | |           
 | |  | | |  | | |  | | |__) | (___   ___  _ __ __ _  __ _ _ __  _ ______ _  __| | ___  _ __ 
 | |  | | |  | | |  | |  _  / \___ \ / _ \| '__/ _` |/ _` | '_ \| |_  / _` |/ _` |/ _ \| '__|
 | |__| | |__| | |__| | | \ \ ____) | (_) | | | (_| | (_| | | | | |/ / (_| | (_| | (_) | |   
 |_____/ \____/ \____/|_|  \_\_____/ \___/|_|  \__, |\__,_|_| |_|_/___\__,_|\__,_|\___/|_|   
                                                __/ |                                        
                                               |___/                                         
     github - Obentemiller V(0.1)
     <powered by minilm>
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
    /.-.-.-.-.-.-.-.-.\\       
   /.-.-.-.-.-.-.-.-.-.\\      
  /.-.-.-.-.-.-.-.-.-.-.\\     
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

# Carregar modelo e tokenizer
tokenizer = AutoTokenizer.from_pretrained("sentence-transformers/paraphrase-MiniLM-L6-v2")
model = AutoModel.from_pretrained("sentence-transformers/paraphrase-MiniLM-L6-v2")

# Função para obter embedding
def get_embedding(text):
    inputs = tokenizer(text, return_tensors='pt', truncation=True, padding=True)
    with torch.no_grad():
        outputs = model(**inputs)
    return outputs.last_hidden_state.mean(dim=1).squeeze().cpu().numpy()

# Função para categorizar arquivos dentro de subcategorias
def categorizar_subcategoria(arquivos, categorias, pasta):
    categoria_embeddings = {categoria: get_embedding(categoria) for categoria in categorias}

    for arquivo in arquivos:
        file_embedding = get_embedding(arquivo)
        
        similaridades = {categoria: cosine_similarity([file_embedding], [embedding])[0][0] for categoria, embedding in categoria_embeddings.items()}
        
        melhor_categoria = max(similaridades, key=similaridades.get)
        destino = os.path.join(pasta, melhor_categoria)
        os.makedirs(destino, exist_ok=True)
        shutil.move(arquivo, os.path.join(destino, os.path.basename(arquivo)))

# Função para organizar por tipo de arquivo e depois por conteúdo
def organizar_por_categoria(pasta):
    tipos_arquivos = {
        'Documentos': {'.pdf', '.doc', '.docx', '.txt', '.ppt', '.pptx', '.xls', '.xlsx', '.odt', '.odp', '.ods'},
        'Imagens': {'.png', '.jpg', '.jpeg', '.gif', '.bmp', '.tiff','.jfif'},
        'Vídeos': {'.mp4', '.avi', '.mov', '.mkv', '.wmv', '.flv'},
        'Atalhos e Executáveis': {'.lnk', '.exe'}
    }
    
    subcategorias = {
        'Documentos': ["Finanças", "Marketing", "Educação", "Saúde", "Tecnologia", "Ciência", "Negócios"],
        'Imagens': ["Fotos", "Arte", "Design", "Natureza", "Eventos"],
        'Vídeos': ["Filmes", "Tutoriais", "Entretenimento", "Música", "Esportes"],
        'Atalhos e Executáveis': ["Softwares", "Jogos", "Ferramentas"]
    }

    arquivos_por_tipo = {tipo: [] for tipo in tipos_arquivos}

    for arquivo in os.listdir(pasta):
        caminho_arquivo = os.path.join(pasta, arquivo)
        
        if os.path.isfile(caminho_arquivo):
            formato = os.path.splitext(arquivo)[1].lower()

            for tipo, extensoes in tipos_arquivos.items():
                if formato in extensoes:
                    arquivos_por_tipo[tipo].append(caminho_arquivo)
                    break
            else:
                if 'Outros' not in arquivos_por_tipo:
                    arquivos_por_tipo['Outros'] = []
                arquivos_por_tipo['Outros'].append(caminho_arquivo)

    for tipo, arquivos in arquivos_por_tipo.items():
        tipo_pasta = os.path.join(pasta, tipo)
        os.makedirs(tipo_pasta, exist_ok=True)
        
        if tipo in subcategorias:
            categorizar_subcategoria(arquivos, subcategorias[tipo], tipo_pasta)
        else:
            for arquivo in arquivos:
                shutil.move(arquivo, os.path.join(tipo_pasta, os.path.basename(arquivo)))

    print("Organização concluída!")
    exibir_ascii_lista_com_atraso(lista_ascii)

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
