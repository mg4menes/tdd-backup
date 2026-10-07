// Copyright 2026 Marcello da Silva Mangueira

#include "backup.hpp"  // NOLINT(build/include_subdir)
#include <string>
#include <cassert>

/**
 * @brief Verifica se poder realizar o backup do arquivo.
 * A função carrega o arquivo com base no caminho dado
 * Caso não possa ser aberto, retorna IMPOSSIVEL.
 * Caso contrário, retorna SALVAR.
 * @param caminho_backup_parm Caminho do arquivo que será verificado.
 * @pre caminho_backup_parm deve conter algum conteúdo.
 * @return Algum valor do Enum Resultado (IMPOSSIVEL ou SALVAR).
 */
Backup::Resultado Backup::FazerBackup(const std::string caminho_backup_parm,
                                      const std::string& caminho_hd,
                                      const std::string& caminho_pendrive) {

  assert(!caminho_backup_parm.empty());  // Assertiva de entrada (pre)

  std::ifstream arquivo_parm(caminho_backup_parm.c_str());
  if (!arquivo_parm.good()) {
    return IMPOSSIVEL;
  }

  return NADA;
}