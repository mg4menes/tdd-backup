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
  enum ComparacaoDataHD {
    HD_DESATUALIZADO,
    HD_IGUAL,
    HD_ATUALIZADO
  };
  Resultado FazerBackup(bool backup_solicitado,
                        const std::string caminho,
                        const std::string& caminho_hd,
                        const std::string& caminho_pendrive);

 private:
  bool CopiarDado(const std::string& origem,
                  const std::string& destino);
  ComparacaoDataHD CompararData(const std::string& caminho_hd,
                               const std::string& caminho_pendrive);
};

#endif  // TDD_BACKUP_BACKUP_HPP_
