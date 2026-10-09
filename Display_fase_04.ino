void desenharFase4() {
    inicializarMatriz();

    // ── Extração de Estados (Baseado na topologia de init_fase_4) ──────────
    short int vA    = values[0]; 
    short int vB    = values[1]; 
    short int vD    = values[2]; 
    short int vE    = values[3]; 
    short int vF    = values[4]; 
    
    short int vAND1 = values[5]; 
    short int vOR1  = values[6]; 
    short int vNF   = values[7]; 
    
    short int vAND2 = values[8]; 
    short int vOR2  = values[9]; 
    short int vS    = values[10]; 

    // ── Variáveis Booleanas (Blink IHM) ──────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_D = !(selected_input == 2 && !blink_state);
    bool show_E = !(selected_input == 3 && !blink_state);
    bool show_F = !(selected_input == 4 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ────────────────────────
    int rowA = 4;
    int rowB = 9;
    int rowD = 16;
    int rowE = 21;
    int rowF = 25;

    // ── Planejamento Espacial Relativo (Âncoras X) ───────────────────────────
    int colNOT  = 9;
    int colL1   = 21; // AND1, OR1
    int colL2   = 33; // AND2
    int colL3   = 45; // OR2
    int colLED  = 55; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowAND1 = rowA + 1; // 5
    int rowOR1  = rowD + 1; // 17
    int rowNOTF = rowF - 2; // 23
    int rowAND2 = rowE + 1; // 22
    int rowOR2  = rowD - 3; // 13

    // Pinos de Saída (Out)
    int outAND1 = rowAND1 + 2; // 7
    int outOR1  = rowOR1 + 3;  // 20
    int outNF   = rowNOTF + 2; // 25
    int outAND2 = rowAND2 + 2; // 24
    int outOR2  = rowOR2 + 3;  // 16

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND1 = rowAND1 + 1; int in2_AND1 = rowAND1 + 3; // 6, 8
    int in1_OR1  = rowOR1 + 1;  int in2_OR1  = rowOR1 + 5;  // 18, 22
    int in1_AND2 = rowAND2 + 1; int in2_AND2 = rowAND2 + 3; // 23, 25
    int in1_OR2  = rowOR2 + 1;  int in2_OR2  = rowOR2 + 5;  // 14, 18

    // Colunas de Derivação Vertical
    int colDerivL1 = colL1 - 6; // 15
    int colDerivL2 = colL2 - 3; // 30
    int colDerivL3 = colL3 - 5; // 40

    // ── 2. Alimentação de Entradas e Inversor ────────────────────────────────
    if (show_A) { MH(0, colDerivL1, rowA, vA); }
    if (show_B) { MH(0, colDerivL1, rowB, vB); }
    if (show_D) { MH(0, colDerivL1, rowD, vD); }
    if (show_E) { MH(0, colDerivL1, rowE, vE); }
    if (show_F) { MH(0, colNOT - 1, rowF, vF); }

    // NOT_F e linha direta perfeita até o pino Base do AND2
    mpNOT(colNOT, rowNOTF, 2);  
    MH(colNOT + 3, colL2 - 1, outNF, vNF); 

    // Ajuste Vertical para as entradas do Estágio 1
    MV(colDerivL1, rowA, in1_AND1, vA); MH(colDerivL1 + 1, colL1 - 1, in1_AND1, vA);
    MV(colDerivL1, rowB, in2_AND1, vB); MH(colDerivL1 + 1, colL1 - 1, in2_AND1, vB);

    MV(colDerivL1, rowD, in1_OR1, vD);  MH(colDerivL1 + 1, colL1 - 1, in1_OR1, vD);
    MV(colDerivL1, rowE, in2_OR1, vE);  MH(colDerivL1 + 1, colL1 - 1, in2_OR1, vE);

    // ── 3. Estágio Lógico 1: AND(A,B) e OR(D,E) ──────────────────────────────
    mpAND(colL1, rowAND1, 2);
    mpOR(colL1, rowOR1, 2);

    // ── 4. Roteamento Intermediário (Sub-árvore Inferior) ────────────────────
    // OR1 (Y=20) desce para o Pino Topo do AND2 (Y=23)
    MH(colL1 + 8, colDerivL2, outOR1, vOR1); 
    MV(colDerivL2, outOR1, in1_AND2, vOR1); 
    MH(colDerivL2 + 1, colL2 - 1, in1_AND2, vOR1);

    // ── 5. Estágio Lógico 2: AND(OR1, NF) ────────────────────────────────────
    mpAND(colL2, rowAND2, 2);

    // ── 6. Estágio Lógico Final (Convergência Global) ────────────────────────
    // AND1 (Y=7) desce para o Pino Topo do OR2 (Y=14)
    MH(colL1 + 5, colDerivL3, outAND1, vAND1); 
    MV(colDerivL3, outAND1, in1_OR2, vAND1); 
    MH(colDerivL3 + 1, colL3 - 1, in1_OR2, vAND1);
    
    // AND2 (Y=24) sobe para o Pino Base do OR2 (Y=18)
    MH(colL2 + 5, colDerivL3, outAND2, vAND2); 
    MV(colDerivL3, outAND2, in2_OR2, vAND2); 
    MH(colDerivL3 + 1, colL3 - 1, in2_OR2, vAND2);

    // ── 7. Porta OR2 Final ───────────────────────────────────────────────────
    mpOR(colL3, rowOR2, 2);

    // ── 8. Saída → LED indicador ─────────────────────────────────────────────
    MH(colL3 + 8, colLED - 1, outOR2, vS);
    
    for (int r = outOR2 - 1; r <= outOR2 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 9. Flush no Framebuffer ──────────────────────────────────────────────
    renderizarComCores();
}