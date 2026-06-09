void desenharFase5() {
    inicializarMatriz();

    // ── Extração de Estados (Baseado na topologia de init_fase_5) ──────────
    short int vA  = values[0];  short int vB  = values[1];
    short int vC  = values[2];  short int vD  = values[3];
    short int vE  = values[4];  short int vF  = values[5];

    // ── Variáveis Booleanas ──────────────────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_C = !(selected_input == 2 && !blink_state);
    bool show_D = !(selected_input == 3 && !blink_state);
    bool show_E = !(selected_input == 4 && !blink_state);
    bool show_F = !(selected_input == 5 && !blink_state);
    
    short int vNA = values[6];  short int vNC = values[7];  short int vND = values[8];
    
    short int vAND1 = values[9];  // AND(NA, B)
    short int vAND2 = values[10]; // AND(NC, ND)
    short int vAND3 = values[11]; // AND(E, F)
    
    short int vAND4 = values[12]; // AND(AND1, AND2)
    short int vAND5 = values[13]; // AND(AND4, AND3)
    short int vS    = values[14]; // Saída Final (S)

    // ── 1. Planejamento Espacial Relativo (Âncoras) ────────────────────────
    int rowA = 3;  int rowB = 8;
    int rowC = 13; int rowD = 18;
    int rowE = 23; int rowF = 28;

    int colNOT = 7;
    int colL1  = 19; 
    int colL2  = 33; 
    int colL3  = 47; 

    // Colunas de derivação calculadas dinamicamente
    int colDeriv1 = colL1 - 3; // Coluna X=16
    int colDeriv2 = colL2 - 3; // Coluna X=30
    int colDeriv3 = colL3 - 3; // Coluna X=44
    int colLED    = colL3 + 8; // Coluna X=55

    // Linhas operacionais das portas calculadas por interpolação
    int rowAND1 = rowA + 1; // 4
    int rowAND2 = rowC + 1; // 14
    int rowAND3 = rowE + 1; // 24
    int rowAND4 = rowB;     // 8
    int rowAND5 = rowAND4 + ((rowAND3 - rowAND4) / 2); // 16

    // ── Condições aplicadas no Roteamento Inicial ────────────────────────────
    if (show_A) { MH(0, colNOT - 1, rowA, vA);  }
    if (show_B) { MH(0, colDeriv1,  rowB, vB);  }
    if (show_C) { MH(0, colNOT - 1, rowC, vC);  }
    if (show_D) { MH(0, colNOT - 1, rowD, vD);  }
    if (show_E) { MH(0, colDeriv1,  rowE, vE);  }
    if (show_F) { MH(0, colDeriv1,  rowF, vF);  }

    // ── 2. Renderização das Entradas e Inversores ─────────────────────────
    mpNOT(colNOT, rowA - 2, 2);
    mpNOT(colNOT, rowC - 2, 2);
    mpNOT(colNOT, rowD - 2, 2);
                
    // Condução das saídas invertidas até a Coluna de Derivação 1
    MH(colNOT + 3, colDeriv1, rowA, vNA);
    MH(colNOT + 3, colDeriv1, rowC, vNC);
    MH(colNOT + 3, colDeriv1, rowD, vND);

    // ── 3. Estágio Lógico 1 (Portas de Front-end) ─────────────────────────
    // Roteamento Ortogonal -> AND1 (NA, B)
    MV(colDeriv1, rowA, rowAND1 + 1, vNA);      MH(colDeriv1, colL1 - 1, rowAND1 + 1, vNA);
    MV(colDeriv1, rowB, rowAND1 + 3, vB);   MH(colDeriv1, colL1 - 1, rowAND1 + 3, vB);
    mpAND(colL1, rowAND1, 2);
    
    // Roteamento Ortogonal -> AND2 (NC, ND)
    MV(colDeriv1, rowC, rowAND2, vNC);      MH(colDeriv1, colL1 - 1, rowAND2, vNC);
    MV(colDeriv1, rowD, rowAND2 + 2, vND);  MH(colDeriv1, colL1 - 1, rowAND2 + 2, vND);
    mpAND(colL1, rowAND2, 2);
    
    // Roteamento Ortogonal -> AND3 (E, F)
    MV(colDeriv1, rowE, rowAND3, vE);       MH(colDeriv1, colL1 - 1, rowAND3, vE);
    MV(colDeriv1, rowF, rowAND3 + 2, vF);   MH(colDeriv1, colL1 - 1, rowAND3 + 2, vF);
    mpAND(colL1, rowAND3, 2);

    // ── 4. Estágio Lógico 2 (Conjunção Intermediária) ─────────────────────
    int rowOutAND1 = rowAND1 + 1;
    int rowOutAND2 = rowAND2 + 1;
    
    // Alimentando AND4 com AND1 e AND2
    MH(colL1 + 5, colDeriv2, rowOutAND1, vAND1);
    MV(colDeriv2, rowOutAND1, rowAND4, vAND1);
    MH(colDeriv2, colL2 - 1, rowAND4, vAND1);
    
    MH(colL1 + 5, colDeriv2, rowOutAND2, vAND2);
    MV(colDeriv2, rowOutAND2, rowAND4 + 2, vAND2);
    MH(colDeriv2, colL2 - 1, rowAND4 + 2, vAND2);
    
    mpAND(colL2, rowAND4, 2);

    // ── 5. Estágio Lógico 3 (Estágio de Potência/Saída Final) ─────────────
    int rowOutAND4 = rowAND4 + 1;
    int rowOutAND3 = rowAND3 + 1;
    
    // Alimentando AND5 com AND4 e AND3
    MH(colL2 + 5, colDeriv3, rowOutAND4, vAND4);
    MV(colDeriv3, rowOutAND4, rowAND5, vAND4);
    MH(colDeriv3, colL3 - 1, rowAND5, vAND4);
    
    MH(colL1 + 5, colDeriv3, rowOutAND3, vAND3); 
    MV(colDeriv3, rowOutAND3, rowAND5 + 2, vAND3);
    MH(colDeriv3, colL3 - 1, rowAND5 + 2, vAND3);
    
    mpAND(colL3, rowAND5, 2);

    // ── 6. Roteamento até o Indicador de Estado Lógico (LED S) ────────────
    int rowOutAND5 = rowAND5 + 1;
    MH(colL3 + 5, colLED - 1, rowOutAND5, vS);
    
    // Malha do LED S de verificação (matriz de 3x3 no fim da trilha)
    for (int r = rowOutAND5 - 1; r <= rowOutAND5 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 7. Flush no Framebuffer ───────────────────────────────────────────
    renderizarComCores();
}