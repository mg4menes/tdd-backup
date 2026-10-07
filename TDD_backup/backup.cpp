// Copyright 2026 Marcello da Silva Mangueira

#include "backup.hpp"  // NOLINT(build/include_subdir)
#include <sys/stat.h>
#include <string>
#include <cassert>

/**
 * @brief Verifica se pode realizar o backup do arquivo e realiza o backup.
 * A função primeiro verifica se Backup.parm existe
 * Caso não possa ser exista, retorna IMPOSSIVEL.
 * Caso contrário, verifica qual o caminho do backup
 * @param caminho_backup_parm Caminho do arquivo que será verificado.
 * @param caminho_hd Caminho do arquivo do HD
 * @param caminho_pendrive Caminho do arquivo do Pendrive.
 * @pre caminho_backup_parm deve conter algum conteúdo.
 * @return Algum valor do Enum Resultado (IMPOSSIVEL, SALVAR, NADA ou ERRO).
 */
Backup::Resultado Backup::FazerBackup(bool backup_solicitado,
                                      const std::string caminho_backup_parm,
                                      const std::string& caminho_hd,
                                      const std::string& caminho_pendrive) {
  assert(!caminho_backup_parm.empty());  // Assertiva de entrada (pre)

  std::ifstream arquivo_parm(caminho_backup_parm.c_str());
  std::ifstream input_arquivo_hd(caminho_hd.c_str());
  std::ifstream input_arquivo_pendrive(caminho_pendrive.c_str());

  if (!arquivo_parm.good()) {  // Verifica se backup_parm existe
    return IMPOSSIVEL;
  }

  // Resgata os tempos do Pendrive e HD e compara
  Resultado retorno_avaliacao = AvaliaData(backup_solicitado,
                                           caminho_hd,
                                           caminho_pendrive);
  if (retorno_avaliacao == NADA && !backup_solicitado) {
    return NADA;
  }

  if (retorno_avaliacao == RESTAURAR) {
    return RESTAURAR;
  }

  if (!backup_solicitado) {
    return ERRO;
  }

  // Backup do HD para Pendrive
  if (input_arquivo_hd.good() && !input_arquivo_pendrive.good()) {
    CopiarDado(caminho_hd, caminho_pendrive);
    return SALVAR;
  }

  return retorno_avaliacao;
}

/**
 * @brief Avalia o estado atual das datas dos arquivos.
 * Pega os caminhos do HD e Pendrive e copia os dados necessário.
 * @param caminho_hd Caminho do arquivo do HD
 * @param caminho_pendrive Caminho do arquivo do Pendrive.
 * @pre caminho_hd deve conter algum conteúdo.
 * @pre caminho_pendrive deve conter algum conteúdo.
 * @return Algum valor do Enum Resultado (SALVAR, NADA ou ERRO).
 */
Backup::Resultado Backup::AvaliaData(bool backup_solicitado,
                                  const std::string& caminho_hd,
                                  const std::string& caminho_pendrive) {
  assert(!caminho_hd.empty());
  assert(!caminho_pendrive.empty());

  int comparacao = CompararData(caminho_hd, caminho_pendrive);
  if (comparacao == HD_ATUALIZADO) {
    CopiarDado(caminho_hd, caminho_pendrive);
    return SALVAR;
  } else if (comparacao == HD_IGUAL) {
    return NADA;
  } else if (comparacao == HD_DESATUALIZADO) {
    if (backup_solicitado) {
      return ERRO;
    } else {
      CopiarDado(caminho_pendrive, caminho_hd);
      return RESTAURAR;
    }
  }

  return ERRO;
}

/**
 * @brief Compara os horários dos 2 arquivos dos caminhhos
 * @param caminho_hd Caminho do arquivo do HD
 * @param caminho_pendrive Caminho do arquivo do Pendrive.
 * @pre caminho_hd deve conter algum conteúdo.
 * @pre caminho_pendrive deve conter algum conteúdo.
 * @return Algum valor do AvaliacaoDataHD (HD_DESATUALIZADO, HD_IGUAL, HD_ATUALIZADO).
 */
Backup::ComparacaoDataHD Backup::CompararData(const std::string& caminho_hd,
                                  const std::string& caminho_pendrive) {
  assert(!caminho_hd.empty());
  assert(!caminho_pendrive.empty());

  struct stat tempo_pendrive;
  stat(caminho_pendrive.c_str(), &tempo_pendrive);
  struct stat tempo_hd;
  stat(caminho_hd.c_str(), &tempo_hd);

  if (tempo_pendrive.st_mtime < tempo_hd.st_mtime) {
    return HD_ATUALIZADO;
  } else if (tempo_pendrive.st_mtime == tempo_hd.st_mtime) {
    return HD_IGUAL;
  } else {
    return HD_DESATUALIZADO;
  }
}

/**
 * @brief Copia os dados do arquivo origem para o destino.
 * @param caminho_origem Caminho do arquivo a ser copiado.
 * @param caminho_destino Caminho do arquivo a ser colado.
 * @pre caminho_origem deve conter algum conteúdo.
 * @pre caminho_destino deve conter algum conteúdo.
 * @return Retorna true caso a cópia seja bem sucedida.
 */
bool Backup::CopiarDado(const std::string& caminho_origem,
                        const std::string& caminho_destino) {
  assert(!caminho_origem.empty());
  assert(!caminho_destino.empty());

  std::ifstream arquivo_origem(caminho_origem.c_str());
  std::ofstream arquivo_destino(caminho_destino.c_str());

  if (!arquivo_origem.good() || !arquivo_destino.good()) {
    return false;
  }

  arquivo_destino << arquivo_origem.rdbuf();
  return arquivo_destino.good();
}
