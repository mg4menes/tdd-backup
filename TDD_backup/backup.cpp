// Copyright 2026 Marcello da Silva Mangueira

#include "backup.hpp"

Backup::Resultado Backup::FazerBackup(const std::string caminho_backup_parm){
  std::ifstream arquivo(caminho_backup_parm.c_str());

  if (!arquivo.good()) {
    return Resultado::IMPOSSIVEL;
  }

  return SALVAR;
}