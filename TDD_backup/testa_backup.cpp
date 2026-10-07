// Copyright 2026 Marcello da Silva Mangueira

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

TEST_CASE("Teste 1 - Backup.parm não existe") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";


  // Garantir que Backup.parm não existe
  std::remove(caminho_parm.c_str());

  Backup::Resultado acao = backup.FazerBackup(caminho_parm, caminho_hd, caminho_pendrive);

  REQUIRE(acao == backup.Resultado::IMPOSSIVEL);
}

TEST_CASE("Teste 2 - Backup.parm existe, quer fazer backup e arquivo somente no HD") {
  Backup backup = CriarBackup();
  // Caminhos Backup.parm, HD e Pendrive
  const char* caminho_hd = "../HD/arquivo.txt";
  const char* caminho_pendrive = "../Pendrive/arquivo.txt";
  const std::string caminho_parm = "../Backup.parm";


  // Garantir que não há arquivo em Pendrive
  std::remove(caminho_pendrive);

  // Garantir que há arquivo com conteúdo em HD
  std::ofstream arquivo_hd(caminho_hd);
  arquivo_hd << "conteudo do arquivo";
  arquivo_hd.close();
  
  Backup::Resultado acao = backup.FazerBackup(caminho_parm, caminho_hd, caminho_pendrive);

  REQUIRE(acao == backup.Resultado::SALVAR);
}
