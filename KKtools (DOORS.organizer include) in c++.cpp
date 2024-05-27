#include <iostream>
#include <conio.h>
#include <windows.h>
#include <random>
#include <shlobj.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <Shellapi.h>
#include <unordered_map>
#include <Lmcons.h>

#define RESET "\033[0m"
#define GREEN "\033[32m"

bool S_DAF;

std::string getUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    }
    else {
        return std::string();
    }
}


static bool IsRunAsAdmin() {
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup))
    {
        dwError = GetLastError();
    }
    else {
        if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
        {
            dwError = GetLastError();
        }
        FreeSid(pAdministratorsGroup);
    }

    return (dwError == ERROR_SUCCESS && fIsRunAsAdmin);
}

static LPCWSTR CharToLPCWSTR(const char* text) {
    size_t size = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (size == 0) {
        return NULL;
    }
    wchar_t* wideText = new wchar_t[size];
    if (!wideText) {
        return NULL;
    }
    if (!MultiByteToWideChar(CP_UTF8, 0, text, -1, wideText, static_cast<int>(size))) {
        delete[] wideText;
        return NULL;
    }
    return wideText;
}

static void executeCommandWithAdminPermission(const std::string& command) {
    if (IsRunAsAdmin()) {
        system(command.c_str());
    }
    else {
        std::cout << "Este comando requer permissão de administrador.\n";
        std::cout << "Digite 'y' para executar como administrador ou 'n' para cancelar: ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            SHELLEXECUTEINFO shellExecuteInfo = { 0 };
            shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            shellExecuteInfo.lpVerb = L"runas";
            shellExecuteInfo.lpFile = L"cmd.exe";
            shellExecuteInfo.lpParameters = CharToLPCWSTR(("/c " + command).c_str());
            shellExecuteInfo.nShow = SW_NORMAL;

            if (!ShellExecuteEx(&shellExecuteInfo)) {
                std::cerr << "Erro ao executar o comando como administrador." << std::endl;
            }
            else {
                WaitForSingleObject(shellExecuteInfo.hProcess, INFINITE);
                CloseHandle(shellExecuteInfo.hProcess);
            }
        }
        else {
            std::cout << "Comando cancelado.\n";
        }
    }
}

class CommandExecutor {
private:
    std::string command;
    std::string tag;

public:
    CommandExecutor(std::string cmd, std::string t) : command(cmd), tag(t) {}

    void execute() const {
        std::cout << "Executando comando: " << command << std::endl;
        executeCommandWithAdminPermission(command);
    }

    std::string getCommand() const {
        return command;
    }

    std::string getTag() const {
        return tag;
    }
};



static void randowblimp(int ini, int fim, int probabilityOfStar) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(ini, fim);

    for (int i = ini; i <= fim; i++) {
        if (distrib(gen) <= probabilityOfStar) {
            std::cout << "*";
        }
        else {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

static void brightness_transition(int speed) {
    int i = 0;
    while (i != 100) {
        i += speed;
        randowblimp(0, 100, i);
    }

    while (i != 0) {
        i -= speed;
        randowblimp(0, 100, i);
    }
}

static void limparTela() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }
    if (!SetConsoleCursorPosition(hConsole, coordScreen)) {
        return;
    }
}




void saveCommands(const std::vector<CommandExecutor>& commands) {
    std::ofstream outputFile("commands.txt");
    if (outputFile.is_open()) {
        for (const CommandExecutor& cmd : commands) {
            outputFile << cmd.getTag() << "|" << cmd.getCommand() << std::endl; // Tag antes do comando
        }
        outputFile.close();
    }
    else {
        std::cerr << "Erro ao abrir o arquivo para salvar os comandos." << std::endl;
    }
}


std::string usernamekk = getUsername();

std::string logo = R"(
  _  ___  ___              _     
 | |/ / |/ / |            | |    
 | ' /| ' /| |_ ___   ___ | |___ 
 |  ; |  ; | __/ _ \ / _ \| / __|
 | . \| . \| |_ (_) | (_) | \__ \
 |_|\_\_|\_\\__\___/ \___/|_|___/
  github - Obentemiller                              
                                 )";

std::string ascii_art0 = R"(
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
 /______/__________\___o_\
 \_______________________/                                                             
)";

std::string ascii_art1 = R"(
    ._________________.       
    |.---------------.|       
    ||               ||       
    ||   ATÉ MAIS!   ||        
    ||  Obentemiller ||       
    ||               ||       
    ||_______________||       
    /.-.-.-.-.-.-.-.-.\     
   /.-.-.-.-.-.-.-.-.-.\    
  /.-.-.-.-.-.-.-.-.-.-.\    
 /______/__________\___o_\
 \_______________________/                                                             
)";

std::string ascii_art2 = R"(
     _________________     
    /                  \      
   /                    \       
  /                      \      
 /_____________obrigado __\     
 |------------------------|
 \________________________/                                                             
)";



static void finalização() {
    Sleep(1000);
    limparTela();
    std::cout << ascii_art0 << std::endl;
    Sleep(1000);
    limparTela();
    std::cout << ascii_art1 << std::endl;
    Sleep(1000);
    limparTela();
    std::cout << ascii_art2 << std::endl;
    std::cout << "até mais " << usernamekk << "." << std::endl;
}

static void renova_interface() {
    limparTela();
    std::cout << logo << std::endl;
}


std::wstring OpenDirectoryDialog(HWND hwndOwner) {
    std::wstring selectedDirectory;

    BROWSEINFO browseInfo = { 0 };
    browseInfo.hwndOwner = hwndOwner;
    browseInfo.lpszTitle = L"Selecione um diretório";
    browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&browseInfo);
    if (pidl != nullptr) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path)) {
            selectedDirectory = path;
        }
        CoTaskMemFree(pidl);
    }

    return selectedDirectory;
}

std::wstring GetFileExtension(const std::wstring& filename) {
    size_t pos = filename.find_last_of(L'.');
    if (pos != std::wstring::npos) {
        return filename.substr(pos);
    }
    return L"";
}

const std::map<std::wstring, std::wstring> tiposDeArquivos{
    {L".pdf", L"Documentos"},
    {L".odt", L"Documentos"},
    {L".doc", L"Documentos"},
    {L".docx", L"Documentos"},
    {L".xls", L"Documentos"},
    {L".xlsx", L"Documentos"},
    {L".ppt", L"Documentos"},
    {L".pptx", L"Documentos"},
    {L".txt", L"Documentos"},
    {L".rtf", L"Documentos"},
    {L".csv", L"Planilhas"},
    {L".html", L"Páginas da Web"},
    {L".htm", L"Páginas da Web"},
    {L".xml", L"Arquivos XML"},
    {L".css", L"Folhas de Estilo"},
    {L".js", L"Scripts JavaScript"},
    {L".java", L"Códigos Java"},
    {L".cpp", L"Códigos C++"},
    {L".h", L"Cabeçalhos de C/C++"},
    {L".cs", L"Códigos C#"},
    {L".php", L"Códigos PHP"},
    {L".asp", L"Páginas ASP.NET"},
    {L".jsp", L"Páginas JSP"},
    {L".py", L"Códigos Python"},
    {L".rb", L"Códigos Ruby"},
    {L".sql", L"Scripts SQL"},
    {L".svg", L"Gráficos Vetoriais"},
    {L".psd", L"Arquivos Photoshop"},
    {L".ai", L"Arquivos Illustrator"},
    {L".indd", L"Documentos InDesign"},
    {L".odt", L"Documentos OpenDocument"},
    {L".ods", L"Planilhas OpenDocument"},
    {L".odp", L"Apresentações OpenDocument"},
    {L".odg", L"Gráficos OpenDocument"},
    {L".ogg", L"Arquivos de Áudio Ogg"},
    {L".mp3", L"Arquivos de Áudio MP3"},
    {L".wav", L"Arquivos de Áudio WAV"},
    {L".flac", L"Arquivos de Áudio FLAC"},
    {L".aac", L"Arquivos de Áudio AAC"},
    {L".wma", L"Arquivos de Áudio WMA"},
    {L".mp4", L"Vídeos MP4"},
    {L".avi", L"Vídeos AVI"},
    {L".mkv", L"Vídeos Matroska"},
    {L".mov", L"Vídeos QuickTime"},
    {L".wmv", L"Vídeos Windows Media"},
    {L".flv", L"Vídeos Flash"},
    {L".m4v", L"Vídeos M4V"},
    {L".webm", L"Vídeos WebM"},
    {L".gif", L"Imagens GIF"},
    {L".jpg", L"Imagens JPEG"},
    {L".jpeg", L"Imagens JPEG"},
    {L".png", L"Imagens PNG"},
    {L".jfif", L"Imagens JFIF"},
    {L".bmp", L"Imagens BMP"},
    {L".tiff", L"Imagens TIFF"},
    {L".ps", L"Documentos PostScript"},
    {L".eps", L"Imagens EPS"},
    {L".ai", L"Arquivos Illustrator"},
    {L".svg", L"Gráficos Vetoriais SVG"},
    {L".zip", L"Arquivos Compactados ZIP"},
    {L".rar", L"Arquivos Compactados RAR"},
    {L".7z", L"Arquivos Compactados 7z"},
    {L".tar", L"Arquivos Compactados Tar"},
    {L".gz", L"Arquivos Compactados Gzip"},
    {L".exe", L"Executáveis"},
    {L".dll", L"Bibliotecas Dinâmicas"},
    {L".bat", L"Arquivos de Lote"},
    {L".sh", L"Scripts Shell"},
    {L".jar", L"Arquivos Java"},
    {L".class", L"Arquivos de Classe Java"},
    {L".deb", L"Arquivos Debian Package"},
    {L".rpm", L"Arquivos RPM Package"},
    {L".apk", L"Arquivos Android Package"},
    {L".ipa", L"Arquivos iOS App"},
    {L".dmg", L"Arquivos Apple Disk Image"},
    {L".lnk", L"Atalhos"},
    {L".txt", L"Documentos TXT"}
    // Adicione mais tipos de arquivos conforme necessário, por favor...
};

void organizarArquivosPorTipo(const std::wstring& diretorio) {
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW((diretorio + L"\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring extensao = GetFileExtension(findData.cFileName);
                if (tiposDeArquivos.find(extensao) != tiposDeArquivos.end()) {
                    std::wstring destino = diretorio + L"\\" + tiposDeArquivos.at(extensao);
                    if (!CreateDirectoryW(destino.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
                        std::wcerr << L"Erro ao criar diretório: " << destino << L". Erro: " << GetLastError() << std::endl;
                        continue;
                    }

                    std::wstring origem = diretorio + L"\\" + findData.cFileName;
                    std::wstring novoDestino = destino + L"\\" + findData.cFileName;
                    if (!MoveFileW(origem.c_str(), novoDestino.c_str())) {
                        std::wcerr << L"Erro ao mover arquivo: " << origem << L". Erro: " << GetLastError() << std::endl;
                    }
                }
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }
}



static char selectgrafic() {
    char in = ' ';
    bool modo = false;
    while (in != 'q' && in != 27) {
        limparTela();
        std::cout << logo << std::endl;
        std::cout << "     selecione o modo " << (modo ? "<file organizer>" : "<creative tools>") << std::endl;
        std::cout << "  <'q' para continuar>" << "  <'esc' para voltar>" << std::endl;
        in = _getch();
        if (in == 77 || in == 75) {
            modo = !modo;
            S_DAF = modo;
        }
    }
    return(in);
}

static void creative_tools() {
    std::vector<CommandExecutor> commands; // Vetor dinâmico de comandos
    std::ifstream inputFile("commands.txt");
    std::string line;

    // Carregar comandos salvos no arquivo
    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            size_t pos = line.find('|'); // Procurar pelo separador
            std::string tag = line.substr(0, pos); // Obter a tag
            std::string cmd = line.substr(pos + 1); // Obter o comando
            commands.push_back(CommandExecutor(cmd, tag));
        }
        inputFile.close();
    }

    char userInput = ' ';
    int selectedOption = 0;
    bool exitFlag = false;

    bool softwareUpgradeCommandExists = false;
    for (const auto& cmd : commands) {
        if (cmd.getTag() == "software upgrade" && cmd.getCommand() == "winget upgrade --all") {
            softwareUpgradeCommandExists = true;
            break;
        }
    }

    if (!softwareUpgradeCommandExists) {
        commands.insert(commands.begin(), CommandExecutor("winget upgrade --all", "software upgrade"));
    }

    while (!exitFlag) {
        // Exibindo o menu
        renova_interface();

        std::cout << "creative tools" << std::endl;
        std::cout << "Escolha um comando ou adicione/elimine um:\n\n";
        for (size_t i = selectedOption; i < selectedOption + 1; ++i) {
            size_t index = i % commands.size();
            std::cout << "<" << index + 1 << ". (" << commands[index].getTag() << ") - " << commands[index].getCommand() << ">\n" << std::endl;
        }
        std::cout << "Pressione Esc para sair.\n";

        // Processamento da entrada do usuário
        while (!_kbhit()) {}
        userInput = _getch();

        if (userInput == 27) {
            exitFlag = true;
        }
        else if (userInput == 75) {
            selectedOption = (selectedOption - 1 + (commands.empty() ? 1 : commands.size())) % (commands.empty() ? 1 : commands.size());

        }
        else if (userInput == 77) {
            selectedOption = (selectedOption + 1) % commands.size();
        }
        else if (userInput >= '1' && userInput <= '9') {
            size_t index = userInput - '1';
            if (index < commands.size()) {
                commands[index].execute();
            }
            else {
                std::cout << "Comando inválido.\n";
            }
        }
        else if (userInput == 'A' || userInput == 'a') {
            // Adicionar um novo comando
            std::string newCommand;
            std::string newTag;
            std::cout << "Digite a tag do novo comando: ";
            getline(std::cin, newTag);
            std::cout << "Digite o novo comando: ";
            std::getline(std::cin, newCommand);
            if (!newCommand.empty() && !newTag.empty()) {
                commands.push_back(CommandExecutor(newCommand, newTag));
                saveCommands(commands);
            }
            else {
                std::cout << "Entrada inválida. O comando não foi adicionado.\n";
            }
        }
        else if (userInput == 'D' || userInput == 'd') {
            // Eliminar um comando existente
            renova_interface();
            std::cout << "Digite o número do comando que deseja eliminar: ";
            size_t index;
            std::cin >> index;
            std::cin.ignore(); // Limpar o buffer de entrada
            if (index >= 1 && index <= commands.size()) {
                commands.erase(commands.begin() + index - 1);
                saveCommands(commands);
            }
            else {
                std::cout << "Número de comando inválido.\n";
            }
        }
    }
}


static void file_organizer() {
    renova_interface();

    std::wstring diretorio = OpenDirectoryDialog(NULL);

    if (!diretorio.empty()) {
        organizarArquivosPorTipo(diretorio);
        std::wcout << L"Arquivos organizados com sucesso!" << std::endl;
    }
    else {
        std::wcout << L"Nenhum diretório selecionado. Encerrando o programa." << std::endl;
    }
}

int main() { 
    goto inicio_st;
inicio:
    limparTela();
    brightness_transition(1);
    //////////////////////////////////////////////////////////////////////////////////////////////
inicio_st:
    char in = ' ';

    while (in != 'q') {

        if (in == 27) {
            brightness_transition(1);
            limparTela();
            finalização();
            return 0;
        }

        limparTela();
        std::cout << logo << std::endl;
        std::cout << "  <Pressione 'q' para continuar>" << "  <'esc' para fechar>" << std::endl;
        in = _getch();
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    brightness_transition(1);

        char selection = selectgrafic();
        if (selection == 'q') {
            switch (S_DAF) {
            case 0:
                limparTela();
                brightness_transition(1);
                creative_tools();
                brightness_transition(1);
                goto inicio;
                return 0;
            case 1:
                limparTela();
                brightness_transition(1);
                file_organizer();
                brightness_transition(1);
                goto inicio;
                return 0;
            }
        }
        else if (selection == 27) {
            renova_interface();
            goto inicio;
        }
    return 0;
}
