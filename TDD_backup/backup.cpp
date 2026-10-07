// Copyright 2026 Marcello da Silva Mangueira

#include "backup.hpp"  // NOLINT(build/include_subdir)
#include <sys/stat.h>
#include <string>
#include <cassert>

/**
 * @brief Verifica se poder realizar o backup do arquivo e realiza o backup.
 * A função primeiro verifica se Backup.parm existe
 * Caso não possa ser exista, retorna IMPOSSIVEL.
 * Caso contrário, verifica qual o caminho do backup
 * @param caminho_backup_parm Caminho do arquivo que será verificado.
 * @param caminho_hd Caminho do arquivo do HD
 * @param caminho_pendrive Caminho do arquivo do Pendrive.
 * @pre caminho_backup_parm deve conter algum conteúdo.
 * @return Algum valor do Enum Resultado (IMPOSSIVEL, SALVAR ou NADA).
 */
Backup::Resultado Backup::FazerBackup(const std::string caminho_backup_parm,
                                      const std::string& caminho_hd,
                                      const std::string& caminho_pendrive) {
  assert(!caminho_backup_parm.empty());  // Assertiva de entrada (pre)

  std::ifstream arquivo_parm(caminho_backup_parm.c_str());

  std::ifstream input_arquivo_hd(caminho_hd.c_str());
  std::ifstream input_arquivo_pendrive(caminho_pendrive.c_str());

  if (!arquivo_parm.good()) {  // Verifica se backup_parm existe
    return IMPOSSIVEL;
  }

  // Backup do HD para Pendrive
  if (input_arquivo_hd.good() && !input_arquivo_pendrive.good()) {
    std::ofstream output_arquivo_pendrive(caminho_pendrive.c_str());
    output_arquivo_pendrive << input_arquivo_hd.rdbuf();
    return SALVAR;
  }

  // Resgata os tempos do Pendrive e HD e compara
  struct stat tempo_pendrive;
  stat(caminho_pendrive.c_str(), &tempo_pendrive);
  struct stat tempo_hd;
  stat(caminho_hd.c_str(), &tempo_hd);

  if (tempo_pendrive.st_mtime < tempo_hd.st_mtime) {
    std::ofstream output_arquivo_pendrive(caminho_pendrive.c_str());
    output_arquivo_pendrive << input_arquivo_hd.rdbuf();
    return SALVAR;
  }

  return NADA;
}
