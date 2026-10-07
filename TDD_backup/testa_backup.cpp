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
 * @brief Contém todos os caminhos dos arquivos do programa
 */
struct CaminhosTeste {
  const std::string hd = "../HD/arquivo.txt";
  const std::string pendrive = "../Pendrive/arquivo.txt";
  const std::string parm = "../Backup.parm";
};

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
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  ApagarArquivo(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::IMPOSSIVEL);
}

TEST_CASE("Teste 2 - Backup.parm existe, backup e arquivo só no HD") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  ApagarArquivo(caminhos.pendrive);
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}

TEST_CASE("Teste 3 - Backup.parm existe, backup e Pendrive desatualizado") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  EscreverNoArquivo(caminhos.pendrive, "conteudo desatualizado");
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 0);
  ModificarDataArquivo(caminhos.hd, 86400);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}

TEST_CASE("Teste 4 - Backup.parm existe, backup e ambos atualizados") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  EscreverNoArquivo(caminhos.pendrive, "conteudo final");
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 0);
  ModificarDataArquivo(caminhos.hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::NADA);
}

TEST_CASE("Teste 5 - Backup.parm existe, backup e HD desatualizado") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  EscreverNoArquivo(caminhos.pendrive, "conteudo final");
  EscreverNoArquivo(caminhos.hd, "conteudo desatualizado");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 86400);
  ModificarDataArquivo(caminhos.hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 6 - Backup.parm existe, sem backup e arquivo só no HD") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = false;
  ApagarArquivo(caminhos.pendrive);
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 7 - Backup.parm existe, sem backup e Pendrive desatualizado") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = false;
  EscreverNoArquivo(caminhos.pendrive, "conteudo desatualizado");
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 0);
  ModificarDataArquivo(caminhos.hd, 86400);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 8 - Backup.parm existe, sem backup e ambos atualizados") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = false;
  EscreverNoArquivo(caminhos.pendrive, "conteudo final");
  EscreverNoArquivo(caminhos.hd, "conteudo final");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 0);
  ModificarDataArquivo(caminhos.hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::NADA);
}

TEST_CASE("Teste 9 - Backup.parm existe, sem backup e HD desatualizado") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = false;
  EscreverNoArquivo(caminhos.pendrive, "conteudo final");
  EscreverNoArquivo(caminhos.hd, "conteudo desatualizado");
  CriarBackupParm(caminhos.parm);
  ModificarDataArquivo(caminhos.pendrive, 86400);
  ModificarDataArquivo(caminhos.hd, 0);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  REQUIRE(acao == backup.Resultado::RESTAURAR);
}

TEST_CASE("Teste 10 - Backup.parm existe, backup e sem nenhum arquivo") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;

  ApagarArquivo(caminhos.hd);
  ApagarArquivo(caminhos.pendrive);
  CriarBackupParm(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  ApagarArquivo(caminhos.pendrive);
  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 11 - Backup.parm existe, backup e arquivo só no Pendrive") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = true;
  EscreverNoArquivo(caminhos.pendrive, "conteudo final");
  ApagarArquivo(caminhos.hd);
  CriarBackupParm(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  ApagarArquivo(caminhos.hd);
  REQUIRE(acao == backup.Resultado::NADA);
}

TEST_CASE("Teste 12 - Backup.parm existe, sem backup e sem nenhum arquivo") {
  Backup backup = CriarBackup();
  CaminhosTeste caminhos;

  bool fazer_backup = false;
  ApagarArquivo(caminhos.hd);
  ApagarArquivo(caminhos.pendrive);
  CriarBackupParm(caminhos.parm);

  Backup::Resultado acao = backup.FazerBackup(fazer_backup,
                                              caminhos.parm,
                                              caminhos.hd,
                                              caminhos.pendrive);

  ApagarArquivo(caminhos.hd);
  ApagarArquivo(caminhos.pendrive);
  REQUIRE(acao == backup.Resultado::ERRO);
}
