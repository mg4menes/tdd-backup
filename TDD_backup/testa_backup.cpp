// Copyright 2026 Marcello da Silva Mangueira

#include "backup.hpp"  // NOLINT(build/include_subdir)

#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"  // NOLINT(build/include_subdir)

//Cada teste equivale a uma coluna da tabela de decisão
//da aula de testes em caixa fechada.

TEST_CASE("Teste 1 - Backup.parm não existe"){
  Backup backup;

  const std::string caminho = "Backup.parm";
  std::remove(caminho.c_str());

  REQUIRE(backup.FazerBackup(caminho) == backup.Resultado::IMPOSSIVEL);
}