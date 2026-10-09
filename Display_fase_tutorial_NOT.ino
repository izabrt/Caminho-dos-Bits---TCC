void desenharFaseTutorialNOT() {
    inicializarMatriz();

    // Recupera valores lógicos da engine do jogo (Fase de 1 entrada)
    short int vA   = values[0]; // única entrada
    short int vNOT = values[1]; // NOT(A) -> Saída S

    // ── 0. Controle de Seleção (Blink IHM) ────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);

    // ── 1. Geometria Centralizada (Painel 64x32) ──────────────────────────
    int rowNOT_Topo = 13;
    int colNOT      = 29; 

    int rowA = rowNOT_Topo; 
    int rowSaida = rowNOT_Topo + 2; // 15

    // ── 2. Roteamento de Fios ─────────────────────────────────────────────
    if (show_A) {
        MH(0, colNOT - 3, rowSaida, vA);        // Fio central pelo painel
        MV(colNOT - 2, rowA, rowSaida, vA);     // Sobe até a quina do triângulo
        MH(colNOT - 2, colNOT - 1, rowA, vA);   // Entra no NOT
    }

    // ── 3. Renderização da Porta Lógica ───────────────────────────────────
    mpNOT(colNOT, rowNOT_Topo, 2); 

    // ── 4. Roteamento de Saída e Atuador (LED) ────────────────────────────
    MH(colNOT + 3, 50, rowSaida, vNOT);

    for (int r = rowSaida - 1; r <= rowSaida + 1; r++) {
        MH(51, 53, r, vNOT);
    }

    // ── 5. Despacho Gráfico ───────────────────────────────────────────────
    renderizarComCores();

    // ── 6. Camada de Texto Sobreposta (Z-Buffer) ──────────────────────────
    uint16_t cor_letras = display->color565(255, 255, 255);
    display->setFont(&TomThumb);
    display->setTextSize(1);
    display->setTextWrap(false);
    display->setTextColor(cor_letras);

    // Texto inferior "NOT" posicionado sob a porta: X = (64 - (3 * 4)) / 2 = 26
    display->setCursor(26, 24);
    display->print("NOT");

    display->setFont(NULL);
}