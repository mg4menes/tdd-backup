// Copyright 2026 Marcello da Silva Mangueira

#ifndef TDD_BACKUP_BACKUP_HPP_
#define TDD_BACKUP_BACKUP_HPP_

#include <string>
#include <fstream>

class Backup {
 public:
  enum Resultado {
    SALVAR,
    RESTAURAR,
    EXCLUIR,
    ERRO,
    NADA,
    IMPOSSIVEL
  };

  Resultado FazerBackup(const std::string caminho, 
                        const std::string& caminho_hd,
                        const std::string& caminho_pendrive);
};

#endif  // TDD_BACKUP_BACKUP_HPP_
