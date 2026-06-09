void desenharFase8() {
    inicializarMatriz();

    // ── Extração de Estados (Mapeamento exato da init_fase_8) ─────────────
    short int vA       = values[0]; 
    short int vB       = values[1]; 
    short int vC       = values[2]; 
    short int vD       = values[3]; 
    short int vE       = values[4]; 
    short int vF       = values[5]; 
    
    short int vAND_AB  = values[6];  
    short int vAND_CD  = values[7];  
    short int vNAND_CD = values[8];  
    short int vOR_EF   = values[9];  
    
    short int vAND_L3  = values[10]; 
    short int vAND_L4  = values[11]; 
    short int vS       = values[12]; 

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
    int colL1  = 12; // AND_AB, AND_CD, OR_EF
    int colL2  = 20; // Inversor NAND_CD
    int colL3  = 30; // AND_L3
    int colL4  = 42; // AND_L4
    int colLED = 53; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowAND_AB = rowA + 1; // 3
    int rowAND_CD = rowC + 1; // 13
    int rowOR_EF  = rowE - 1; // 21 (Alinha in1 exatamente com E e in2 com F)
    
    int rowNOT_CD = rowAND_CD; // 13
    int rowAND_L3 = 9;
    int rowAND_L4 = 17;

    // Pinos de Saída (Out)
    int outAND_AB  = rowAND_AB + 2; // 5
    int outAND_CD  = rowAND_CD + 2; // 15
    int outNAND_CD = outAND_CD;     // 15
    int outOR_EF   = rowOR_EF + 3;  // 24
    int outAND_L3  = rowAND_L3 + 2; // 11
    int outAND_L4  = rowAND_L4 + 2; // 19

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND_AB = rowAND_AB + 1; // 4
    int in1_AND_CD = rowAND_CD + 1; // 14
    int in1_AND_L3 = rowAND_L3 + 1; int in2_AND_L3 = rowAND_L3 + 3;
    int in1_AND_L4 = rowAND_L4 + 1; int in2_AND_L4 = rowAND_L4 + 3;

    // Colunas de Derivação Vertical
    int colDerivL1 = colL1 - 2; // 10
    int colDerivL3 = colL3 - 2; // 28
    int colDerivL4 = colL4 - 2; // 40

    // ── 2. Alimentação do Estágio 1 ──────────────────────────────────────────
    if (show_A) { MH(0, colDerivL1, rowA, vA); }
    if (show_B) { MH(0, colL1 - 1, rowB, vB);  } // B segue reto
    if (show_C) { MH(0, colDerivL1, rowC, vC); }
    if (show_D) { MH(0, colL1 - 1, rowD, vD);  } // D segue reto
    
    // Devido ao alinhamento perfeito do OR_EF, E e F seguem reto sem derivação
    if (show_E) { MH(0, colL1 - 1, rowE, vE);  } 
    if (show_F) { MH(0, colL1 - 1, rowF, vF);  } 

    // Quedas verticais para os pinos Topo
    MV(colDerivL1, rowA, in1_AND_AB, vA); MH(colDerivL1 + 1, colL1 - 1, in1_AND_AB, vA);
    MV(colDerivL1, rowC, in1_AND_CD, vC); MH(colDerivL1 + 1, colL1 - 1, in1_AND_CD, vC);

    // ── 3. Estágio Lógico 1 (Portas Front-end) ───────────────────────────────
    mpAND(colL1, rowAND_AB, 2); 
    mpAND(colL1, rowAND_CD, 2); 
    mpOR(colL1, rowOR_EF, 2);   

    // ── 4. Estágio Lógico 2 (Inversor do Bloco Central) ──────────────────────
    MH(colL1 + 5, colL2 - 1, outAND_CD, vAND_CD);
    mpNOT(colL2, rowNOT_CD, 2); 

    // ── 5. Estágio Lógico 3 (Conjunção L3) ───────────────────────────────────
    MH(colL1 + 5, colDerivL3, outAND_AB, vAND_AB); 
    MV(colDerivL3, outAND_AB, in1_AND_L3, vAND_AB); 
    MH(colDerivL3 + 1, colL3 - 1, in1_AND_L3, vAND_AB);
    
    MH(colL2 + 3, colDerivL3, outNAND_CD, vNAND_CD); 
    MV(colDerivL3, outNAND_CD, in2_AND_L3, vNAND_CD); 
    MH(colDerivL3 + 1, colL3 - 1, in2_AND_L3, vNAND_CD);
    
    mpAND(colL3, rowAND_L3, 2); 

    // ── 6. Estágio Lógico Final (Conjunção L4) ───────────────────────────────
    MH(colL3 + 5, colDerivL4, outAND_L3, vAND_L3); 
    MV(colDerivL4, outAND_L3, in1_AND_L4, vAND_L3); 
    MH(colDerivL4 + 1, colL4 - 1, in1_AND_L4, vAND_L3);
    
    // A porta OR emite em col + 7 (logo o fio começa em col + 8)
    MH(colL1 + 8, colDerivL4, outOR_EF, vOR_EF); 
    MV(colDerivL4, outOR_EF, in2_AND_L4, vOR_EF); 
    MH(colDerivL4 + 1, colL4 - 1, in2_AND_L4, vOR_EF);

    mpAND(colL4, rowAND_L4, 2); 

    // ── 7. Roteamento até o Indicador de Estado Lógico (LED S) ───────────────
    MH(colL4 + 5, colLED - 1, outAND_L4, vS);
    
    for (int r = outAND_L4 - 1; r <= outAND_L4 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 8. Atualização do Framebuffer ────────────────────────────────────────
    renderizarComCores();
}