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


TEST_CASE("Teste 1 - Backup.parm não existe") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";


  // Garantir que Backup.parm não existe
  std::remove(caminho_parm.c_str());

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
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

  // Decisão de fazer backup
  bool fazer_backup = true;

  // Garantir que não há arquivo em Pendrive
  std::remove(caminho_pendrive);

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo original do hd";
  arquivo_hd.close();

  // Garantir que Backup.parm existe
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
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

  // Decisão de fazer backup
  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  std::ofstream arquivo_pendrive(caminho_pendrive);
  arquivo_pendrive << "conteudo original do pendrive";
  arquivo_pendrive.close();

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo original do hd";
  arquivo_hd.close();

  // Garantir que Backup.parm existe
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();

  // Garantir que o Pendrive está a mais tempo sem alterar
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 86400);

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
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

  // Decisão de fazer backup
  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  std::ofstream arquivo_pendrive(caminho_pendrive);
  arquivo_pendrive << "conteudo original do pendrive";
  arquivo_pendrive.close();

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo original do hd";
  arquivo_hd.close();

  // Garantir que Backup.parm existe
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();

  // Garantir que ambos estão igualmente atualizados
  ModificarDataArquivo(caminho_pendrive, 0);
  ModificarDataArquivo(caminho_hd, 0);

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
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

  // Decisão de fazer backup
  bool fazer_backup = true;

  // Garantir que há arquivo com conteúdo em Pendrive
  std::ofstream arquivo_pendrive(caminho_pendrive);
  arquivo_pendrive << "conteudo original do pendrive";
  arquivo_pendrive.close();

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo original do hd";
  arquivo_hd.close();

  // Garantir que Backup.parm existe
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();

  // Garantir que ambos estão igualmente atualizados
  ModificarDataArquivo(caminho_pendrive, 86400);
  ModificarDataArquivo(caminho_hd, 0);

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}

TEST_CASE("Teste 2 - Backup.parm existe, sem backup e arquivo só no HD") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";

  // Decisão de fazer backup
  bool fazer_backup = false;

  // Garantir que não há arquivo em Pendrive
  std::remove(caminho_pendrive);

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo original do hd";
  arquivo_hd.close();

  // Garantir que Backup.parm existe
  std::ofstream arquivo_parm(caminho_parm);
  arquivo_parm << "arquivo.txt";
  arquivo_parm.close();

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::ERRO);
}