#include <iostream>
#include <string>
#include "FileSystem.hpp"
using namespace std;

// ── Afficher l'aide ──────────────────────────────────────────
void printHelp() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║        File Manager CLI - Commandes      ║\n";
    cout << "╠══════════════════════════════════════════╣\n";
    cout << "║  ls                  Lister le dossier   ║\n";
    cout << "║  cd <dossier>        Naviguer            ║\n";
    cout << "║  cd ..               Remonter            ║\n";
    cout << "║  cat <fichier>       Afficher contenu    ║\n";
    cout << "║  cp <src> <dst>      Copier fichier      ║\n";
    cout << "║  rm <fichier>        Supprimer           ║\n";
    cout << "║  touch <fichier>     Créer fichier vide  ║\n";
    cout << "║  find <nom>          Rechercher          ║\n";
    cout << "║  filter <.ext>       Filtrer extension   ║\n";
    cout << "║  sort name           Trier par nom       ║\n";
    cout << "║  sort size           Trier par taille    ║\n";
    cout << "║  help                Afficher l'aide     ║\n";
    cout << "║  exit                Quitter             ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";
}

//   Parser une ligne en tokens 
vector<string> parseInput(const string& line) {
    vector<string> tokens;
    string token;

    for (char c : line) {
        if (c == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty())
        tokens.push_back(token);

    return tokens;
}

int main(int argc, char* argv[]) {

    // Dossier de depart : argument ou dossier courant
    string startPath = (argc > 1) ? argv[1] : ".";

    cout << "\n🗂️  File Manager CLI\n";
    cout << "📁 Dossier de départ : " << startPath << "\n";
    cout << "   Tapez 'help' pour voir les commandes.\n\n";

    FileSystem fs(startPath);
    fs.list();   // afficher le contenu au demarrage

    string line;

    //  Boucle principale CLI  
    while (true) {
        // Afficher le prompt avec le chemin courant
        cout << "\n[" << fs.getCurrentPath() << "] $ ";
        
        if (!getline(cin, line)) break;  // EOF  
        if (line.empty()) continue;

        vector<string> tokens = parseInput(line);
        if (tokens.empty()) continue;

        string cmd = tokens[0];

        //   ls 
        if (cmd == "ls") {
            fs.list();
        }

        //  cd <dossier>  
        else if (cmd == "cd") {
            if (tokens.size() < 2)
                cout << "❌ Usage : cd <dossier>\n";
            else
                fs.navigate(tokens[1]);
        }

        //  cat <fichier>  
        else if (cmd == "cat") {
            if (tokens.size() < 2)
                cout << "❌ Usage : cat <fichier>\n";
            else
                fs.printFile(tokens[1]);
        }

        //   cp <src> <dst> 
        else if (cmd == "cp") {
            if (tokens.size() < 3)
                cout << "❌ Usage : cp <source> <destination>\n";
            else
                fs.copyFile(tokens[1], tokens[2]);
        }

        //   rm <fichier>  
        else if (cmd == "rm") {
            if (tokens.size() < 2) {
                cout << "❌ Usage : rm <fichier>\n";
            } else {
                // Confirmation avant suppression
                cout << "⚠️  Supprimer \"" << tokens[1] << "\" ? (o/n) : ";
                string confirm;
                getline(cin, confirm);
                if (confirm == "o" || confirm == "O")
                    fs.deleteFile(tokens[1]);
                else
                    cout << "   Annulé.\n";
            }
        }

        //   touch <fichier>  
        else if (cmd == "touch") {
            if (tokens.size() < 2)
                cout << "❌ Usage : touch <fichier>\n";
            else
                fs.createFile(tokens[1]);
        }

        //   find <nom>  
        else if (cmd == "find") {
            if (tokens.size() < 2)
                cout << "❌ Usage : find <nom>\n";
            else
                fs.search(tokens[1]);
        }

        //   filter <.ext>  
        else if (cmd == "filter") {
            if (tokens.size() < 2)
                cout << "❌ Usage : filter <.ext>  ex: filter .txt\n";
            else
                fs.filterByExtension(tokens[1]);
        }

        //   sort name / sort size  
        else if (cmd == "sort") {
            if (tokens.size() < 2) {
                cout << "❌ Usage : sort <name|size>\n";
            } else if (tokens[1] == "name") {
                fs.sortByName();
            } else if (tokens[1] == "size") {
                fs.sortBySize();
            } else {
                cout << "❌ Option inconnue. Utiliser : sort name  ou  sort size\n";
            }
        }

        //  help  
        else if (cmd == "help") {
            printHelp();
        }

        //   exit  
        else if (cmd == "exit" || cmd == "quit") {
            cout << "👋 Au revoir !\n";
            break;
        }

        //  commande inconnue  
        else {
            cout << "❓ Commande inconnue : \"" << cmd << "\". Tapez 'help'.\n";
        }
    }

    return 0;
}