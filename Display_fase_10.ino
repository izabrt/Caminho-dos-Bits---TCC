void desenharFase10() {
    inicializarMatriz();

    // ── Extração de Estados (Mapeamento exato da init_fase_10) ────────────
    short int vA       = values[0]; 
    short int vB       = values[1]; 
    short int vC       = values[2]; 
    short int vD       = values[3]; 
    short int vE       = values[4]; 
    short int vF       = values[5]; 
    
    short int vNA      = values[6];  
    short int vNB      = values[7];  
    short int vNC      = values[8];  
    
    short int vAND1    = values[9];  
    short int vAND2    = values[10]; 
    short int vAND3    = values[11]; 
    short int vAND4    = values[12]; 
    short int vAND5    = values[13]; 
    short int vAND6    = values[14]; 
    
    short int vOR1     = values[15]; 
    short int vOR2     = values[16]; 
    short int vS       = values[17]; 

    // ── Variáveis Booleanas (Blink IHM) ───────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_C = !(selected_input == 2 && !blink_state);
    bool show_D = !(selected_input == 3 && !blink_state);
    bool show_E = !(selected_input == 4 && !blink_state);
    bool show_F = !(selected_input == 5 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ─────────────────────
    int rowA = 2;  
    int rowB = 8;  
    int rowC = 14; 
    int rowD = 20; 
    int rowE = 24; 
    int rowF = 28; 

    // ── Planejamento Espacial Relativo (Âncoras X) ────────────────────────
    int colNOT  = 5;
    int colJmp1 = 8;  // Eixo vertical do Jumper NA
    int colL1   = 14; // AND1, AND2, AND5
    int colJmp2 = 21; // Eixo vertical do Jumper C
    int colL2   = 26; // AND3, AND4, AND6
    int colL3   = 38; // OR1
    int colL4   = 48; // OR2
    int colLED  = 61; 

    // ── Geometria e Terminais das Portas ──────────────────────────────────
    int rowNOT_A = rowA - 2; // 0
    int rowNOT_B = rowB - 2; // 4
    int rowNOT_C = rowC - 2; // 10
    
    int rowAND1  = rowA + 1; // 3
    int rowAND2  = rowB + 3; // 7
    int rowAND5  = rowD + 3; // 19
    
    int rowAND3  = rowAND1 + 2; // 5
    int rowAND4  = rowAND2 + 4; // 9
    int rowAND6  = rowAND5 + 4; // 21
    
    int rowOR1   = rowAND3 + 2; // 7
    int rowOR2   = rowC + 2;    // 14

    // Pinos de Saída (Out)
    int outNA    = rowA;
    int outNB    = rowB;
    int outNC    = rowC;
    int outAND1  = rowAND1 + 2; // 5
    int outAND2  = rowAND2 + 2; // 9
    int outAND5  = rowAND5 + 2; // 21
    int outAND3  = rowAND3 + 2; // 7
    int outAND4  = rowAND4 + 2; // 11
    int outAND6  = rowAND6 + 2; // 23
    int outOR1   = rowOR1 + 3;  // 9
    int outOR2   = rowOR2 + 3;  // 16

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND1 = rowAND1 + 1; int in2_AND1 = rowAND1 + 3;
    int in1_AND2 = rowAND2 + 1; int in2_AND2 = rowAND2 + 3;
    int in1_AND5 = rowAND5 + 1; int in2_AND5 = rowAND5 + 3;
    int in1_AND3 = rowAND3 + 1; int in2_AND3 = rowAND3 + 3;
    int in1_AND4 = rowAND4 + 1; int in2_AND4 = rowAND4 + 3;
    int in1_AND6 = rowAND6 + 1; int in2_AND6 = rowAND6 + 3;
    int in1_OR1  = rowOR1 + 1;  int in2_OR1  = rowOR1 + 5;
    int in1_OR2  = rowOR2 + 1;  int in2_OR2  = rowOR2 + 5;

    // ── 2. Renderização de Entradas, Inversores e Jumpers Iniciais ────────
    // Trilha A e ramificação para NOT_A
    if (show_A) {
        MH(0, 3, rowA, vA);       
        MV(3, rowA, in1_AND1, vA);      
        MH(4, colJmp1 - 1, in1_AND1, vA); // Pré-jumper 
        MH(colJmp1 + 1, colL1 - 1, in1_AND1, vA); // Pós-jumper
        MH(3, 4, rowA, vA);       
    }
    mpNOT(colNOT, rowNOT_A, 2);   
    MH(colNOT + 3, colJmp1, outNA, vNA);  
    MV(colJmp1, outNA, in1_AND2, vNA); // NA desce pelo JUMPER 
    MH(colJmp1, colL1 - 1, in1_AND2, vNA);

    // Trilha B e ramificação para NOT_B
    if (show_B) {
        MH(0, 3, rowB, vB);
        MV(3, rowB, in2_AND2, vB);      
        MH(4, colL1 - 1, in2_AND2, vB);
        MH(3, 4, rowB, vB);
    }
    mpNOT(colNOT, rowNOT_B, 2);  
    MH(colNOT + 3, colJmp1 - 1, outNB, vNB); // Pré-jumper
    MH(colJmp1 + 1, colL1 - 1, outNB, vNB);  // Pós-jumper 

    // Trilha C e ramificação para NOT_C
    int viaC = rowC + 2; // Y=14
    if (show_C) {
        MH(0, 3, rowC, vC);
        MV(3, rowC, viaC, vC);      
        MH(4, colJmp2 - 1, viaC, vC); // Trilha pré-jumper (X=20)
        MH(3, 4, rowC, vC);
    }
    mpNOT(colNOT, rowNOT_C, 2);  
    MH(colNOT + 3, colJmp2 - 1, outNC, vNC); // Trilha pré-jumper (X=20)
    MH(colJmp2 + 1, colL2 - 1, outNC, vNC);  // Trilha pós-jumper 

    // Entradas diretas D, E, F
    if (show_D) { MH(0, 10, rowD, vD); MV(10, rowD, in1_AND5, vD); MH(10, colL1 - 1, in1_AND5, vD); }
    if (show_E) { MH(0, colL1 - 1, rowE, vE); }
    if (show_F) { MH(0, colL2 - 2, rowF, vF); MV(colL2 - 2, rowF, in2_AND6, vF); MH(colL2 - 2, colL2 - 1, in2_AND6, vF); }

    // ── 3. Estágio Lógico 1 (Coluna L1) ───────────────────────────────────
    mpAND(colL1, rowAND1, 2);  
    mpAND(colL1, rowAND2, 2);  
    mpAND(colL1, rowAND5, 2);  

    // ── 4. Roteamento Intermediário L1 -> L2 (Jumpers de Subida) ──────────
    int colDerivL2 = colL2 - 3; // X=23
    MH(colL1 + 5, colDerivL2, outAND1, vAND1); 
    MV(colDerivL2, outAND1, in1_AND3, vAND1); 
    MH(colDerivL2, colL2 - 1, in1_AND3, vAND1);

    // Jumper para C subir cortando NC e AND2
    MH(colL1 + 5, colJmp2 - 1, outAND2, vAND2); // AND2 pré-jumper
    MH(colJmp2 + 1, colDerivL2, outAND2, vAND2); // AND2 pós-jumper
    MV(colDerivL2, outAND2, in1_AND4, vAND2); 
    MH(colDerivL2, colL2 - 1, in1_AND4, vAND2); 

    MH(colJmp2, colJmp2, viaC, vC);  // Início do Jumper de C
    MV(colJmp2, viaC, in2_AND3, vC); // C sobe verticalmente rasgando as trilhas
    MH(colJmp2, colL2 - 1, in2_AND3, vC); 

    int colDerivL2_Inf = colL2 - 2; // X=24
    MH(colL1 + 5, colDerivL2_Inf, outAND5, vAND5); 
    MV(colDerivL2_Inf, outAND5, in1_AND6, vAND5); 
    MH(colDerivL2_Inf, colL2 - 1, in1_AND6, vAND5);

    // ── 5. Estágio Lógico 2 (Coluna L2) ───────────────────────────────────
    mpAND(colL2, rowAND3, 2);  
    mpAND(colL2, rowAND4, 2);  
    mpAND(colL2, rowAND6, 2);  

    // ── 6. Roteamento L2 -> L3 (Convergência do MUX) ──────────────────────
    int colDerivL3 = colL3 - 3; // X=35
    MH(colL2 + 5, colDerivL3, outAND3, vAND3); 
    MV(colDerivL3, outAND3, in1_OR1, vAND3); 
    MH(colDerivL3, colL3 - 1, in1_OR1, vAND3);
    
    MH(colL2 + 5, colDerivL3, outAND4, vAND4); 
    MV(colDerivL3, outAND4, in2_OR1, vAND4); 
    MH(colDerivL3, colL3 - 1, in2_OR1, vAND4);
    
    // AND6 é um sinal de espera, cruza a tela por baixo direto até L4
    int colDerivL4 = colL4 - 2; // X=46
    MH(colL2 + 5, colDerivL4, outAND6, vAND6);  

    // ── 7. Estágio Lógico 3 (Coluna L3) ───────────────────────────────────
    mpOR(colL3, rowOR1, 2);  

    // ── 8. Roteamento L3 -> L4 (Convergência Final) ───────────────────────
    MH(colL3 + 8, colDerivL4, outOR1, vOR1); 
    MV(colDerivL4, outOR1, in1_OR2, vOR1); 
    MH(colDerivL4, colL4 - 1, in1_OR2, vOR1);
    
    MV(colDerivL4, outAND6, in2_OR2, vAND6); 
    MH(colDerivL4, colL4 - 1, in2_OR2, vAND6);

    // ── 9. Estágio Lógico Final (Coluna L4) ───────────────────────────────
    mpOR(colL4, rowOR2, 2);  

    // ── 10. Indicador de Estado Lógico Final (LED S) ──────────────────────
    MH(colL4 + 8, colLED - 1, outOR2, vS);
    for (int r = outOR2 - 1; r <= outOR2 + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 11. Atualização do Framebuffer ────────────────────────────────────
    renderizarComCores();
}