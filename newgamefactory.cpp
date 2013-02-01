#include "newgamefactory.h"
#include "ui_newgamefactory.h"
#include "interfaces/tableconfig.h"

NewGameFactory::NewGameFactory(QWidget *parent)
: QWidget(parent)
, m_ui(new Ui::NewGameFactory) {
  m_ui->setupUi(this);
}

void NewGameFactory::emitTableConfig() {
  TableConfig config;
  
  config.firstPlayer = m_ui->firstMoving->currentIndex() == 0 ? 1 : 2;
  config.gameType = m_ui->gameMode->currentIndex() == 0
      ? WITHOUT_ADDITIONAL_MOVE
      : WITH_ADDITIONAL_MOVE;
      
  config.size.setHeight(m_ui->heightSpinBox->value());
  config.size.setWidth(m_ui->widthSpinBox->value());
  
  emit createTable(config);
}

NewGameFactory::~NewGameFactory() {
  delete m_ui;
}