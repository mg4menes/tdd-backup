// Copyright 2026 Marcello da Silva Mangueira

#include <string>
#include <cstdio>
#include <utime.h>
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

TEST_CASE("Teste 2 - Backup.parm existe, quer backup e arquivo só no HD") {
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

TEST_CASE("Teste 3 - Backup.parm existe, quer backup e Pendrive desatualizado") {
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
  // Modificando manualmente a hora de acesso e modificação para o teste
  struct utimbuf tempo_pendrive;
  tempo_pendrive.actime = 0;  // Dia 0
  tempo_pendrive.modtime = 0;
  utime(caminho_pendrive, &tempo_pendrive);

  struct utimbuf tempo_hd;
  tempo_hd.actime = 86400;
  tempo_hd.modtime = 86400;  // Dia 1
  utime(caminho_pendrive, &tempo_hd);

  Backup::Resultado acao = backup.FazerBackup(caminho_parm,
                                              caminho_hd,
                                              caminho_pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}
