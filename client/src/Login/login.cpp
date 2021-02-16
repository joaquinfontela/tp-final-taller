#include "login.h"

#include <QPixmap>
#include <algorithm>

#include "./Login_autogen/include/ui_login.h"
#include "QFont"
#include "QFontDatabase"
#include "QMessageBox"
#include "QString"
#include "create_window.h"
#include "iostream"
#include "join_window.h"
#include "string.h"

#define BLACK_BUTTON_OR_BOX \
  "* { background-color: rgba(0, 0, 0, 200); color: #ffffff}"
#define WHITESPACE ' '

Login::Login(int& player_id, int& map_id, SocketCommunication& socket)
    : QMainWindow(nullptr),
      ui(new Ui::Login),
      player_id(player_id),
      map_id(map_id),
      socket(socket) {
  ui->setupUi(this);
  QPixmap bkgnd("../media/loginscreen.png");
  bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
  QPalette palette;
  palette.setBrush(QPalette::Background, bkgnd);
  this->setPalette(palette);
  ui->dato_puerto->setPlaceholderText(" PORT");
  ui->dato_ip->setPlaceholderText(" IP");
  ui->dato_puerto->setStyleSheet(BLACK_BUTTON_OR_BOX);
  ui->dato_ip->setStyleSheet(BLACK_BUTTON_OR_BOX);
  ui->button_join->setStyleSheet(BLACK_BUTTON_OR_BOX);
  ui->button_create->setStyleSheet(BLACK_BUTTON_OR_BOX);
}

Login::~Login() { delete ui; }

bool Login::connectToLobby(std::string& host, std::string& port){

  try{
    this->socket.connect(host, port);
    return true;
  }catch(SocketException& e){
    QMessageBox messageBox;
    messageBox.critical(0, "Error", "Server Unavailable.");
    messageBox.setFixedSize(800, 600);
    messageBox.exec();
    return false;
  }
}

bool Login::joinLobby(){
  uint32_t protocol = JOIN_LOBBY;
  uint32_t opcode;
  uint32_t selfID;
  uint32_t mapID;

  // Connection Request
  socket.send(&protocol, sizeof(protocol));
  if(!receiveAvailableMatches()){
      QMessageBox messageBox;
      messageBox.critical(0, "Error", "No Matches Available.");
      messageBox.setFixedSize(800, 600);
      messageBox.exec();
      std::cout<<"[LOGIN] No matches available..."<<std::endl;
      return false;
  }

  join_window jw(this, this->availableMatches);
  jw.setModal(true);
  jw.exec();

  uint32_t lobbyID = jw.get_match_id();

  socket.send(&lobbyID, sizeof(lobbyID));
  socket.receive(&opcode, sizeof(opcode));
  if (opcode != CONNECTED_OK) {
    return false;
  }

  socket.receive(&selfID, sizeof(selfID));
  socket.receive(&mapID, sizeof(mapID));

  this->player_id = selfID;
  this->map_id = mapID;

  return true;
}

void Login::on_button_join_clicked() {
  QString ip = ui->dato_ip->text();
  QString port = ui->dato_puerto->text();
  auto parseSpaces = [](const std::string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    std::string ans = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    size_t start = ans.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : ans.substr(start);
  };
  if (port.isEmpty() || ip.isEmpty()) {
    QMessageBox messageBox;
    messageBox.critical(0, "Error", "Complete all the boxes.");
    messageBox.setFixedSize(800, 600);
    messageBox.exec();
  } else {
    my_ip = parseSpaces(ip.toStdString());
    my_host = parseSpaces(port.toStdString());

    // PARTE DE CONECTARSE A UNA PARTIDA

    if(!connectToLobby(my_ip, my_host)){
      this->player_id = -1;
      return;
    }

    if(!joinLobby()){
      std::cout<<"[LOGIN] Failure to join lobby"<<std::endl;
      this->player_id = -1;
    }

    QApplication::quit();
  }
}

bool Login::createLobby(){

  uint32_t protocol = CREATE_LOBBY;
  uint32_t opcode;
  uint32_t selfID;
  uint32_t map_ID;

  socket.send(&protocol, sizeof(protocol));

  create_window cw(this);
  cw.setModal(true);
  cw.exec();
  map_ID = cw.get_map_id();

  socket.send(&map_ID, sizeof(map_ID));
  socket.receive(&opcode, sizeof(opcode));

  if (opcode != CONNECTED_OK) {
    return false;
  }

  socket.receive(&selfID, sizeof(selfID));
  this->player_id = selfID;
  this->map_id = map_ID;

  return true;

}

void Login::on_button_create_clicked() {
  QString ip = ui->dato_ip->text();
  QString port = ui->dato_puerto->text();
  auto parseSpaces = [](const std::string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    std::string ans = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    size_t start = ans.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : ans.substr(start);
  };
  if (port.isEmpty() || ip.isEmpty()) {
    QMessageBox messageBox;
    messageBox.critical(0, "Error", "Complete all the boxes.");
    messageBox.setFixedSize(800, 600);
    messageBox.exec();
  } else {
    my_ip = parseSpaces(ip.toStdString());
    my_host = parseSpaces(port.toStdString());

    if(!connectToLobby(my_ip, my_host)){
      this->player_id = -1;
      return;
    }

    if(!createLobby()){
      std::cout<<"[LOGIN] Failure creating lobby"<<std::endl;
      this->player_id = -1;
    }

    QApplication::quit();
  }
}

bool Login::receiveAvailableMatches() {
  uint32_t amountOfMatches = 0;
  this->socket.receive(&amountOfMatches, sizeof(amountOfMatches));

  if(amountOfMatches == 0)
    return false;

  for (uint32_t i = 0; i < amountOfMatches; i++) {
    uint32_t matchID;
    this->socket.receive(&matchID, sizeof(matchID));
    this->availableMatches.push_back(matchID);
  }

  return true;
}
