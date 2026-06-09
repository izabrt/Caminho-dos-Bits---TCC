void desenharFase9() {
    inicializarMatriz();

    // ── Extração de Estados (Mapeamento do motor do jogo) ─────────────────
    short int vA       = values[0]; 
    short int vB       = values[1]; 
    short int vC       = values[2]; 
    short int vD       = values[3]; 
    short int vE       = values[4]; 
    short int vF       = values[5]; 
    
    short int vNA      = values[6];  
    short int vAND_BC  = values[7];  
    short int vAND_DE  = values[8];  
    short int vAND_FDE = values[9];  
    short int vOR1     = values[10]; 
    short int vOR2     = values[11]; 
    
    short int vAND_FINAL = values[12]; 
    short int vS         = values[13]; 

    // ── Variáveis Booleanas (Blink IHM) ──────────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);
    bool show_C = !(selected_input == 2 && !blink_state);
    bool show_D = !(selected_input == 3 && !blink_state);
    bool show_E = !(selected_input == 4 && !blink_state);
    bool show_F = !(selected_input == 5 && !blink_state);

    // ── 1. Planejamento Espacial Relativo (Âncoras Y) ────────────────────────
    int rowA = 2;  
    int rowB = 6;  
    int rowC = 10; 
    int rowD = 14; 
    int rowE = 18; 
    int rowF = 26; 

    // ── Planejamento Espacial Relativo (Âncoras X) ───────────────────────────
    int colNOT = 5;
    int colL1  = 12; // AND_BC, AND_DE
    int colL2  = 24; // AND_FDE
    int colL3  = 36; // OR1, OR2
    int colL4  = 48; // AND_FINAL
    int colLED = 57; 

    // ── Geometria e Terminais das Portas ─────────────────────────────────────
    int rowNOT_A   = rowA - 2;
    int rowAND_BC  = rowB;     
    int rowAND_DE  = rowD;     
    int rowAND_FDE = 21;
    int rowOR1     = 7;
    int rowOR2     = 18;
    int rowAND_FINAL = 12;

    // Pinos de Saída (Out)
    int outNA        = rowA;
    int outAND_BC    = rowAND_BC + 2; 
    int outAND_DE    = rowAND_DE + 2; 
    int outAND_FDE   = rowAND_FDE + 2; 
    int outOR1       = rowOR1 + 3; 
    int outOR2       = rowOR2 + 3; 
    int outAND_FINAL = rowAND_FINAL + 2; 

    // Pinos de Entrada (In1 Topo, In2 Base)
    int in1_AND_BC = rowAND_BC + 1; int in2_AND_BC = rowAND_BC + 3;
    int in1_AND_DE = rowAND_DE + 1; int in2_AND_DE = rowAND_DE + 3;
    int in1_AND_FDE= rowAND_FDE+ 1; int in2_AND_FDE= rowAND_FDE+ 3;
    int in1_OR1    = rowOR1 + 1;    int in2_OR1    = rowOR1 + 5;    
    int in1_OR2    = rowOR2 + 1;    int in2_OR2    = rowOR2 + 5;    
    int in1_AND_FINAL= rowAND_FINAL+1; int in2_AND_FINAL= rowAND_FINAL+3;

    // Colunas de Derivação Vertical
    int colDerivL1 = colL1 - 2; // 10
    int colDerivL2 = colL2 - 4; // 20
    int colDerivJmp= colL3 - 5; // 34 (Duto vertical do NA)
    int colDerivL4 = colL4 - 2; // 46

    // ── 2. Renderização de Entradas e Inversor de Borda ──────────────────────
    if (show_A) { MH(0, colNOT - 1, rowA, vA); }
    if (show_B) { MH(0, colDerivL1, rowB, vB); }
    if (show_C) { MH(0, colDerivL1, rowC, vC); }
    if (show_D) { MH(0, colDerivL1, rowD, vD); }
    if (show_E) { MH(0, colDerivL1, rowE, vE); }
    if (show_F) { MH(0, colDerivL2, rowF, vF); } 

    mpNOT(colNOT, rowNOT_A, 2); 

    // ── 3. Estágio Lógico 1 (Front-end) ──────────────────────────────────────
    MV(colDerivL1, rowB, in1_AND_BC, vB); MH(colDerivL1 + 1, colL1 - 1, in1_AND_BC, vB);
    MV(colDerivL1, rowC, in2_AND_BC, vC); MH(colDerivL1 + 1, colL1 - 1, in2_AND_BC, vC);
    mpAND(colL1, rowAND_BC, 2); 

    MV(colDerivL1, rowD, in1_AND_DE, vD); MH(colDerivL1 + 1, colL1 - 1, in1_AND_DE, vD);
    MV(colDerivL1, rowE, in2_AND_DE, vE); MH(colDerivL1 + 1, colL1 - 1, in2_AND_DE, vE);
    mpAND(colL1, rowAND_DE, 2); 

    // ── 4. Estágio Lógico 2 (Cascata F_DE) ───────────────────────────────────
    MH(colL1 + 5, colDerivL2, outAND_DE, vAND_DE); 
    MV(colDerivL2, outAND_DE, in1_AND_FDE, vAND_DE); 
    MH(colDerivL2 + 1, colL2 - 1, in1_AND_FDE, vAND_DE);
    
    MV(colDerivL2, rowF, in2_AND_FDE, vF); 
    MH(colDerivL2 + 1, colL2 - 1, in2_AND_FDE, vF);
    
    mpAND(colL2, rowAND_FDE, 2); 

    // ── 5. Estágio Lógico 3 (Distribuição NA e Portas OR) ────────────────────
    // JUMPER VIRTUAL: Barramento descendente para NA
    MH(colNOT + 3, colDerivJmp, outNA, vNA);  
    MV(colDerivJmp, outNA, in1_OR2, vNA);     
    
    MH(colDerivJmp, colL3 - 1, in1_OR1, vNA); 
    MH(colDerivJmp, colL3 - 1, in1_OR2, vNA); 

    // Jumper: Alimentação Base OR1 (saltando sobre colDerivJmp)
    // O fio vem de Y=8, para no X=33 (deixando gap em 34), e retoma em X=35 caindo para Y=12
    MH(colL1 + 5, colDerivJmp - 1, outAND_BC, vAND_BC);  
    MV(colDerivJmp + 1, outAND_BC, in2_OR1, vAND_BC);
    MH(colDerivJmp + 1, colL3 - 1, in2_OR1, vAND_BC);    
    
    // Alimentação Base OR2
    int colDerivL3_Inf = 31; 
    MH(colL2 + 5, colDerivL3_Inf, outAND_FDE, vAND_FDE); 
    MV(colDerivL3_Inf, outAND_FDE, in2_OR2, vAND_FDE); 
    MH(colDerivL3_Inf, colL3 - 1, in2_OR2, vAND_FDE);

    mpOR(colL3, rowOR1, 2);  
    mpOR(colL3, rowOR2, 2);  

    // ── 6. Estágio Lógico Final (Convergência de Saída) ──────────────────────
    MH(colL3 + 8, colDerivL4, outOR1, vOR1); 
    MV(colDerivL4, outOR1, in1_AND_FINAL, vOR1); 
    MH(colDerivL4 + 1, colL4 - 1, in1_AND_FINAL, vOR1);
    
    MH(colL3 + 8, colDerivL4, outOR2, vOR2); 
    MV(colDerivL4, outOR2, in2_AND_FINAL, vOR2); 
    MH(colDerivL4 + 1, colL4 - 1, in2_AND_FINAL, vOR2);

    mpAND(colL4, rowAND_FINAL, 2); 

    // ── 7. Roteamento até o Indicador de Estado Lógico (LED S) ───────────────
    MH(colL4 + 5, colLED - 1, outAND_FINAL, vS);
    
    for (int r = outAND_FINAL - 1; r <= outAND_FINAL + 1; r++) {
        MH(colLED, colLED + 2, r, vS);
    }

    // ── 8. Flush no Framebuffer ──────────────────────────────────────────────
    renderizarComCores();
}