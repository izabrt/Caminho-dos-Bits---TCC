void desenharFase6() {
    inicializarMatriz();

    // ── Extração de Estados ──────────────────────────────────────────────────
    short int vA       = values[0]; 
    short int vB       = values[1]; 
    short int vC       = values[2]; 
    short int vD       = values[3]; 
    short int vE       = values[4]; 
    short int vF       = values[5]; 
    
    short int vNA      = values[6];  
    short int vNF      = values[7];  
    
    short int vAND_AB  = values[8];  
    short int vOR_CD   = values[9];  
    short int vAND_CD  = values[10]; 
    short int vNAND_CD = values[11]; 
    short int vXOR     = values[12]; 
    
    short int vAND_L3  = values[13]; 
    short int vAND_L4  = values[14]; 
    short int vAND_L5  = values[15]; 
    short int vS       = values[16]; 

    // ── Variáveis Booleanas (Blink IHM) ──────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_C = !(selected_input == 2 && !blink_state);
    bool show_D = !(selected_input == 3 && !blink_state);
    bool show_E = !(selected_input == 4 && !blink_state);
    bool show_F = !(selected_input == 5 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ────────────────────────
    int rowA = 2;  int rowB = 6;  
    int rowC = 10; int rowD = 18; 
    int rowE = 22; int rowF = 28; 

    // ── Planejamento Espacial Relativo (Âncoras X) ───────────────────────────
    int colNOT  = 5;
    int colL1   = 12; // AND_AB, OR_CD, AND_CD
    int colNAND = 20; // Inversor do AND_CD
    int colL2   = 26; // XOR_L2
    int colL3   = 34; // AND_L3
    int colL4   = 42; // AND_L4
    int colL5   = 50; // AND_L5
    int colLED  = 58; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowNOT_A = rowA - 2;
    int rowNOT_F = rowF - 2;

    int rowAND_AB = 3;
    int rowOR_CD  = rowC - 1; // 11
    int rowAND_CD = rowD - 1; // 17
    
    int rowNOT_CD = (rowAND_CD + 2) - 2; // Alinhado à saída do AND_CD (17)

    int rowXOR    = 14;
    int rowAND_L3 = 9;
    int rowAND_L4 = 15;
    int rowAND_L5 = 21;

    // Pinos de Saída (Out)
    int outNA = rowA;
    int outNF = rowF;
    int outAND_AB  = rowAND_AB + 2; 
    int outOR_CD   = rowOR_CD + 3;  
    int outAND_CD  = rowAND_CD + 2; 
    int outNAND_CD = outAND_CD;     
    int outXOR     = rowXOR + 2;    
    int outAND_L3  = rowAND_L3 + 2; 
    int outAND_L4  = rowAND_L4 + 2; 
    int outAND_L5  = rowAND_L5 + 2; 

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND_AB = rowAND_AB + 1; int in2_AND_AB = rowAND_AB + 3;
    int in1_OR_CD  = rowOR_CD + 1;  int in2_OR_CD  = rowOR_CD + 5;
    int in1_AND_CD = rowAND_CD + 1; int in2_AND_CD = rowAND_CD + 3;
    int in1_XOR    = rowXOR + 1;    int in2_XOR    = rowXOR + 3;
    int in1_AND_L3 = rowAND_L3 + 1; int in2_AND_L3 = rowAND_L3 + 3;
    int in1_AND_L4 = rowAND_L4 + 1; int in2_AND_L4 = rowAND_L4 + 3;
    int in1_AND_L5 = rowAND_L5 + 1; int in2_AND_L5 = rowAND_L5 + 3;

    // Colunas de Derivação Vertical
    int colDerivCD   = 8;
    int colDerivE    = 40;
    int colDerivL1   = 24;
    int colDerivL2   = 32;
    int colDerivL3   = 40;
    int colDerivL4   = 48;

    // ── 2. Entradas Iniciais e Inversores de Borda ───────────────────────────
    if (show_A) { MH(0, colNOT - 1, rowA, vA); }
    if (show_B) { MH(0, colL1 - 1, rowB, vB);  }
    if (show_C) { MH(0, colDerivCD - 1, rowC, vC); }
    if (show_D) { MH(0, colDerivCD - 1, rowD, vD); }
    if (show_E) { MH(0, colDerivE, rowE, vE);  }
    if (show_F) { MH(0, colNOT - 1, rowF, vF); }

    mpNOT(colNOT, rowNOT_A, 2); 
    mpNOT(colNOT, rowNOT_F, 2);

    // O barramento NF (Y=28) estendido até a coluna final
    MH(colNOT + 3, colDerivL4, outNF, vNF);

    // ── 3. Subcircuito: Fan-out de C e D para formação do XOR ───────────────
    MH(colDerivCD, colL1 - 1, in1_OR_CD, vC); 
    MV(colDerivCD, rowC, in1_AND_CD, vC); MH(colDerivCD + 1, colL1 - 1, in1_AND_CD, vC);

    MV(colDerivCD + 1, rowD, in2_OR_CD, vD); MH(colDerivCD + 2, colL1 - 1, in2_OR_CD, vD); 
    MV(colDerivCD, rowD, in2_AND_CD, vD); MH(colDerivCD + 1, colL1 - 1, in2_AND_CD, vD);

    // ── 4. Estágio Lógico 1 (Coluna L1) ──────────────────────────────────────
    int colDerivNA = 11;
    MH(colNOT + 3, colDerivNA, outNA, vNA); 
    MV(colDerivNA, outNA, in1_AND_AB, vNA); 

    mpAND(colL1, rowAND_AB, 2); 
    mpOR(colL1, rowOR_CD, 2);   
    mpAND(colL1, rowAND_CD, 2); 

    // ── 5. Estágio Lógico 2: Conclusão do XOR (NAND) ─────────────────────────
    MH(colL1 + 5, colNAND - 1, outAND_CD, vAND_CD);
    mpNOT(colNAND, rowNOT_CD, 2); 

    // ── 6. Estágio Lógico 3: Convergência do XOR (Coluna L2) ─────────────────
    MH(colL1 + 8, colDerivL1, outOR_CD, vOR_CD);   
    MV(colDerivL1, outOR_CD, in1_XOR, vOR_CD);   
    MH(colDerivL1 + 1, colL2 - 1, in1_XOR, vOR_CD);

    MH(colNAND + 3, colDerivL1, outNAND_CD, vNAND_CD); 
    MV(colDerivL1, outNAND_CD, in2_XOR, vNAND_CD); 
    MH(colDerivL1 + 1, colL2 - 1, in2_XOR, vNAND_CD);
    
    mpAND(colL2, rowXOR, 2); 

    // ── 7. Estágio Lógico 4: Cascata com AND_AB (Coluna L3) ──────────────────
    MH(colL1 + 5, colDerivL2, outAND_AB, vAND_AB); 
    MV(colDerivL2, outAND_AB, in1_AND_L3, vAND_AB); 
    MH(colDerivL2 + 1, colL3 - 1, in1_AND_L3, vAND_AB);

    MH(colL2 + 5, colDerivL2, outXOR, vXOR);   
    MV(colDerivL2, outXOR, in2_AND_L3, vXOR);   
    MH(colDerivL2 + 1, colL3 - 1, in2_AND_L3, vXOR);
    
    mpAND(colL3, rowAND_L3, 2);

    // ── 8. Estágio Lógico 5: Cascata com Entrada E (Coluna L4) ───────────────
    MH(colL3 + 5, colDerivL3, outAND_L3, vAND_L3); 
    MV(colDerivL3, outAND_L3, in1_AND_L4, vAND_L3); 
    MH(colDerivL3 + 1, colL4 - 1, in1_AND_L4, vAND_L3);

    MV(colDerivE, rowE, in2_AND_L4, vE);      
    MH(colDerivE + 1, colL4 - 1, in2_AND_L4, vE);
    
    mpAND(colL4, rowAND_L4, 2);

    // ── 9. Estágio Lógico Final: Cascata com NOT_F (Coluna L5) ───────────────
    MH(colL4 + 5, colDerivL4, outAND_L4, vAND_L4); 
    MV(colDerivL4, outAND_L4, in1_AND_L5, vAND_L4); 
    MH(colDerivL4 + 1, colL5 - 1, in1_AND_L5, vAND_L4);

    MV(colDerivL4, outNF, in2_AND_L5, vNF);      
    MH(colDerivL4 + 1, colL5 - 1, in2_AND_L5, vNF);
    
    mpAND(colL5, rowAND_L5, 2);

    // ── 10. Indicador de Estado Lógico (LED S) ───────────────────────────────
    MH(colL5 + 5, colLED - 1, outAND_L5, vS);
    for (int r = outAND_L5 - 1; r <= outAND_L5 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 11. Flush no Framebuffer ─────────────────────────────────────────────
    renderizarComCores();
}