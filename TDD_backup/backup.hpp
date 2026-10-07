// Copyright 2026 Marcello da Silva Mangueira

#ifndef TDD_BACKUP_HPP
#define TDD_BACKUP_HPP

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

  Resultado FazerBackup(const std::string caminho);
};

#endif  // TDD_BACKUP_HPP