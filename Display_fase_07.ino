void desenharFase7() {
    inicializarMatriz();

    // ── Extração de Estados (Mapeamento exato da init_fase_7) ─────────────
    short int vA       = values[0]; 
    short int vB       = values[1]; 
    short int vC       = values[2]; 
    short int vD       = values[3]; 
    short int vE       = values[4]; 
    short int vF       = values[5]; 
    
    short int vNA      = values[6];  
    short int vNC      = values[7];  
    short int vNE      = values[8];  
    
    short int vAND_AB  = values[9];  
    short int vNAND_AB = values[10]; 
    short int vAND_CD  = values[11]; 
    short int vAND_EF  = values[12]; 
    
    short int vAND_L3  = values[13]; 
    short int vAND_L4  = values[14]; 
    short int vS       = values[15]; 

    // ── Variáveis Booleanas (Blink IHM) ──────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_C = !(selected_input == 2 && !blink_state);
    bool show_D = !(selected_input == 3 && !blink_state);
    bool show_E = !(selected_input == 4 && !blink_state);
    bool show_F = !(selected_input == 5 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ────────────────────────
    int rowA = 2;  int rowB = 6;  
    int rowC = 12; int rowD = 16; 
    int rowE = 22; int rowF = 26; 

    // ── Planejamento Espacial Relativo (Âncoras X) ───────────────────────────
    int colNOT = 5;
    int colL1  = 12; // AND_AB, AND_CD, AND_EF
    int colL2  = 20; // Inversor NAND_AB
    int colL3  = 30; // AND_L3
    int colL4  = 42; // AND_L4
    int colLED = 53; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowNOT_A  = rowA - 2;
    int rowNOT_C  = rowC - 2;
    int rowNOT_E  = rowE - 2;

    int rowAND_AB = rowA + 1; // 3
    int rowAND_CD = rowC + 1; // 13
    int rowAND_EF = rowE + 1; // 23
    
    int rowNOT_AB = rowAND_AB; // 3
    int rowAND_L3 = 9;
    int rowAND_L4 = 17;

    // Pinos de Saída (Out)
    int outNA = rowA;
    int outNC = rowC;
    int outNE = rowE;
    
    int outAND_AB  = rowAND_AB + 2; 
    int outNAND_AB = outAND_AB;     
    int outAND_CD  = rowAND_CD + 2; 
    int outAND_EF  = rowAND_EF + 2; 
    int outAND_L3  = rowAND_L3 + 2; 
    int outAND_L4  = rowAND_L4 + 2; 

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND_AB = rowAND_AB + 1; // 4 (Row B é 6, entra direto)
    int in1_AND_CD = rowAND_CD + 1; // 14 (Row D é 16, entra direto)
    int in1_AND_EF = rowAND_EF + 1; // 24 (Row F é 26, entra direto)
    
    int in1_AND_L3 = rowAND_L3 + 1; int in2_AND_L3 = rowAND_L3 + 3;
    int in1_AND_L4 = rowAND_L4 + 1; int in2_AND_L4 = rowAND_L4 + 3;

    // Colunas de Derivação Vertical
    int colDerivL1 = colL1 - 2; // 10
    int colDerivL3 = colL3 - 2; // 28
    int colDerivL4 = colL4 - 2; // 40

    // ── 2. Entradas Iniciais e Inversores Primários ──────────────────────────
    if (show_A) { MH(0, colNOT - 1, rowA, vA); }
    if (show_B) { MH(0, colL1 - 1, rowB, vB);  }
    if (show_C) { MH(0, colNOT - 1, rowC, vC); }
    if (show_D) { MH(0, colL1 - 1, rowD, vD);  }
    if (show_E) { MH(0, colNOT - 1, rowE, vE); }
    if (show_F) { MH(0, colL1 - 1, rowF, vF);  }

    mpNOT(colNOT, rowNOT_A, 2); 
    mpNOT(colNOT, rowNOT_C, 2); 
    mpNOT(colNOT, rowNOT_E, 2); 

    // ── 3. Estágio Lógico 1 (Portas AND de Front-end) ────────────────────────
    MH(colNOT + 3, colDerivL1, outNA, vNA); 
    MV(colDerivL1, outNA, in1_AND_AB, vNA); 
    MH(colDerivL1 + 1, colL1 - 1, in1_AND_AB, vNA);

    MH(colNOT + 3, colDerivL1, outNC, vNC); 
    MV(colDerivL1, outNC, in1_AND_CD, vNC); 
    MH(colDerivL1 + 1, colL1 - 1, in1_AND_CD, vNC);

    MH(colNOT + 3, colDerivL1, outNE, vNE); 
    MV(colDerivL1, outNE, in1_AND_EF, vNE); 
    MH(colDerivL1 + 1, colL1 - 1, in1_AND_EF, vNE);

    mpAND(colL1, rowAND_AB, 2); 
    mpAND(colL1, rowAND_CD, 2); 
    mpAND(colL1, rowAND_EF, 2); 

    // ── 4. Estágio Lógico 2 (Inversor Intermediário) ─────────────────────────
    MH(colL1 + 5, colL2 - 1, outAND_AB, vAND_AB);
    mpNOT(colL2, rowNOT_AB, 2); 

    // ── 5. Estágio Lógico 3 (Conjunção L3) ───────────────────────────────────
    MH(colL2 + 3, colDerivL3, outNAND_AB, vNAND_AB); 
    MV(colDerivL3, outNAND_AB, in1_AND_L3, vNAND_AB); 
    MH(colDerivL3 + 1, colL3 - 1, in1_AND_L3, vNAND_AB);
    
    MH(colL1 + 5, colDerivL3, outAND_CD, vAND_CD); 
    MV(colDerivL3, outAND_CD, in2_AND_L3, vAND_CD); 
    MH(colDerivL3 + 1, colL3 - 1, in2_AND_L3, vAND_CD);
    
    mpAND(colL3, rowAND_L3, 2); 

    // ── 6. Estágio Lógico Final (Conjunção L4) ───────────────────────────────
    MH(colL3 + 5, colDerivL4, outAND_L3, vAND_L3); 
    MV(colDerivL4, outAND_L3, in1_AND_L4, vAND_L3); 
    MH(colDerivL4 + 1, colL4 - 1, in1_AND_L4, vAND_L3);
    
    MH(colL1 + 5, colDerivL4, outAND_EF, vAND_EF); 
    MV(colDerivL4, outAND_EF, in2_AND_L4, vAND_EF); 
    MH(colDerivL4 + 1, colL4 - 1, in2_AND_L4, vAND_EF);

    mpAND(colL4, rowAND_L4, 2); 

    // ── 7. Roteamento até o Indicador de Estado Lógico (LED S) ───────────────
    MH(colL4 + 5, colLED - 1, outAND_L4, vS);
    
    for (int r = outAND_L4 - 1; r <= outAND_L4 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 8. Flush no Framebuffer ──────────────────────────────────────────────
    renderizarComCores();
}