#include "separar.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(Separar, UmCampo)
{
    const auto campos = separar("ana", ';');

    ASSERT_EQ(campos.size(), 1u);
    EXPECT_EQ(campos[0], "ana");
}

TEST(Separar, VariosCampos)
{
    const auto campos = separar("ana;32;Recife", ';');

    ASSERT_EQ(campos.size(), 3u);
    EXPECT_EQ(campos[0], "ana");
    EXPECT_EQ(campos[1], "32");
    EXPECT_EQ(campos[2], "Recife");
}

TEST(Separar, CampoVazioNoMeio)
{
    const std::vector<std::string> esperado{"ana", "", "Recife"};

    EXPECT_EQ(separar("ana;;Recife", ';'), esperado);
}

TEST(Separar, LinhaVazia)
{
    EXPECT_TRUE(separar("", ';').empty());
}
