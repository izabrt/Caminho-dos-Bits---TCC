void desenharFase2() {
    inicializarMatriz();

    // ── Extração de Estados ──────────────────────────────────────────────────
    short int vA    = values[0];  
    short int vB    = values[1];  
    short int vF    = values[2];  
    short int vNB   = values[3];  
    short int vAND1 = values[4];  
    short int vAND2 = values[5];  
    short int vOR   = values[6];  
    short int vS    = values[7]; // Saída Final (Buffer S)

    // ── Variáveis Booleanas (Blink IHM) ──────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_F = !(selected_input == 2 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ────────────────────────
    int rowA = 4;
    int rowB = 15;
    int rowF = 24;

    // ── Planejamento Espacial Relativo (Âncoras X) ───────────────────────────
    int colNOT  = 9;
    int colL1   = 23; // AND1, AND2
    int colL2   = 42; // OR
    int colLED  = 53; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowNOT_B = rowB - 2; // 13
    int rowAND1  = rowA + 3; // 7
    int rowAND2  = rowF - 5; // 19
    int rowOR    = rowB - 2; // 13

    // Pinos de Saída (Out)
    int outNB    = rowB;        // 15
    int outAND1  = rowAND1 + 2; // 9
    int outAND2  = rowAND2 + 2; // 21
    int outOR    = rowOR + 3;   // 16

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND1 = rowAND1 + 1; // 8
    int in2_AND1 = rowAND1 + 3; // 10
    int in1_AND2 = rowAND2 + 1; // 20
    int in2_AND2 = rowAND2 + 3; // 22
    int in1_OR   = rowOR + 1;   // 14
    int in2_OR   = rowOR + 5;   // 18

    // Colunas de Derivação Vertical
    int colDerivB_dir = colNOT - 5; // 4 (B contorna antes do NOT)
    int colDerivA_F   = colNOT - 1; // 8
    int colDerivNB    = colNOT + 5; // 14
    int colDerivL2    = colL1 + 8;  // 31

    // ── 2. Alimentação de Entradas e Inversores ──────────────────────────────
    if (show_A) { MH(0, colDerivA_F, rowA, vA); }
    if (show_B) { MH(0, colNOT - 1, rowB, vB);  } // Avança até o NOT_B
    if (show_F) { MH(0, colDerivA_F, rowF, vF); }

    mpNOT(colNOT, rowNOT_B, 2);

    // ── 3. Roteamento L1 (Entradas para AND1 e AND2) ─────────────────────────
    // A -> AND1 (Pino Topo)
    MV(colDerivA_F, rowA, in1_AND1, vA); 
    MH(colDerivA_F + 1, colL1 - 1, in1_AND1, vA);

    // NB -> AND1 (Pino Base)
    MH(colNOT + 3, colDerivNB, outNB, vNB);
    MV(colDerivNB, outNB, in2_AND1, vNB);
    MH(colDerivNB + 1, colL1 - 1, in2_AND1, vNB);

    // B Direto -> AND2 (Pino Topo)
    MV(colDerivB_dir, rowB, in1_AND2, vB);
    MH(colDerivB_dir + 1, colL1 - 1, in1_AND2, vB);

    // F -> AND2 (Pino Base)
    MV(colDerivA_F, rowF, in2_AND2, vF);
    MH(colDerivA_F + 1, colL1 - 1, in2_AND2, vF);

    // ── 4. Estágio Lógico 1 (AND1 e AND2) ────────────────────────────────────
    mpAND(colL1, rowAND1, 2);
    mpAND(colL1, rowAND2, 2);

    // ── 5. Roteamento L2 (AND1 e AND2 para OR) ───────────────────────────────
    MH(colL1 + 5, colDerivL2, outAND1, vAND1);
    MV(colDerivL2, outAND1, in1_OR, vAND1);
    MH(colDerivL2 + 1, colL2 - 1, in1_OR, vAND1);

    MH(colL1 + 5, colDerivL2, outAND2, vAND2);
    MV(colDerivL2, outAND2, in2_OR, vAND2);
    MH(colDerivL2 + 1, colL2 - 1, in2_OR, vAND2);

    // ── 6. Estágio Lógico Final (OR) ─────────────────────────────────────────
    mpOR(colL2, rowOR, 2);

    // ── 7. Saída OR → LED indicador ──────────────────────────────────────────
    MH(colL2 + 8, colLED - 1, outOR, vS);
    
    for (int r = outOR - 1; r <= outOR + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 8. Flush no Framebuffer ──────────────────────────────────────────────
    renderizarComCores();
}