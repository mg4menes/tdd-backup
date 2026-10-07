// Copyright 2026 Marcello da Silva Mangueira

#include <utime.h>
#include <string>
#include <cstdio>
#include "backup.hpp"  // NOLINT(build/include_subdir)

#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"  // NOLINT(build/include_subdir)


// Cada teste equivale a uma coluna da tabela de decisão
// da aula de testes em caixa fechada.

// O conteúdo que está sendo espelhado entre o HD e o
// pendrive é um arquivo.txt

/**
 * @brief Cria um objeto do tipo Backup
 * @return Retorna o objeto do tipo Backup
 */
Backup CriarBackup() {
  Backup backup;
  return backup;
}

/**
 * @brief Modifica a data de modificação e acesso de um arquivo.
 * Recebe os argumentos de caminho do arquivo e
 * o horario em que ele será alterado.
 * @param caminho_arquivo Caminho do arquivo que terá data modificada.
 * @param horario Horário da modificação.
 */
void ModificarDataArquivo(const std::string& caminho_arquivo, time_t horario) {
  struct utimbuf tempo_arquivo;
  tempo_arquivo.actime = horario;
  tempo_arquivo.modtime = horario;
  utime(caminho_arquivo.c_str(), &tempo_arquivo);
}

/**
 * @brief Cria o arquivo Backup.parm.
 * @param caminho_parm Caminho do arquivo do Backup.parm.
 */
void CriarBackupParm(const std::string caminho_parm) {
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();
}

/**
 * @brief Apagar o arquivo dado no caminho.
 * @param caminho Caminho do arquivo a ser apagado.
 */
void ApagarArquivo(const std::string caminho) {
  std::remove(caminho.c_str());
}

void EscreverNoArquivo(const std::string caminho, const std::string conteudo) {
  std::ofstream arquivo(caminho);
  arquivo << conteudo;
  arquivo.close();
}

TEST_CASE("Teste 1 - Backup.parm não existe") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = true;
  ApagarArquivo(caminho_parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::IMPOSSIVEL);
}

TEST_CASE("Teste 2 - Backup.parm existe, backup e arquivo só no HD") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = true;
  ApagarArquivo(caminho_pendrive);

  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}

TEST_CASE("Teste 3 - Backup.parm existe, backup e Pendrive desatualizado") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  EscreverNoArquivo(caminho_pendrive, "conteudo desatualizado");
  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  // Garantir que o Pendrive está a mais tempo sem alterar
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 86400);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}

TEST_CASE("Teste 4 - Backup.parm existe, backup e ambos atualizados") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  EscreverNoArquivo(caminho_pendrive, "conteudo final");
  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  // Garantir que ambos estão igualmente atualizados
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::NADA);
}

TEST_CASE("Teste 5 - Backup.parm existe, backup e HD desatualizado") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  EscreverNoArquivo(caminho_pendrive, "conteudo final");
  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo desatualizado");
  CriarBackupParm(caminho_parm);

  // Garantir que ambos estão igualmente atualizados
  ModificarDataArquivo(caminho_pendrive, 86400);
  ModificarDataArquivo(caminho_hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 6 - Backup.parm existe, sem backup e arquivo só no HD") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = false;
  ApagarArquivo(caminho_pendrive);

  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 7 - Backup.parm existe, sem backup e Pendrive desatualizado") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = false;

  // Garantir que há arquivo com conteúdo em Pendrive
  EscreverNoArquivo(caminho_pendrive, "conteudo desatualizado");
  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  // Garantir que o Pendrive está a mais tempo sem alterar
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 86400);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 8 - Backup.parm existe, sem backup e ambos atualizados") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  bool fazer_backup = false;

  // Garantir que há arquivo com conteúdo em Pendrive
  EscreverNoArquivo(caminho_pendrive, "conteudo final");
  // Garantir que há arquivo com conteúdo em HD
  EscreverNoArquivo(caminho_hd, "conteudo final");
  CriarBackupParm(caminho_parm);

  // Garantir que o Pendrive está a mais tempo sem alterar
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::NADA);
}
