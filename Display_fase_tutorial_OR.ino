void desenharFaseTutorialOR() {
    inicializarMatriz();

    // Recupera valores lógicos da engine do jogo (Fase de 2 entradas)
    short int vA  = values[0]; // entrada A (cima)
    short int vB  = values[1]; // entrada B (baixo)
    short int vOR = values[2]; // OR(A, B) -> Saída S

    // ── 0. Controle de Seleção (Blink IHM) ────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);

    // ── 1. Geometria Centralizada (Painel 64x32) ──────────────────────────
    int rowOR_Topo = 12; 
    int colOR      = 28; 

    int rowA = rowOR_Topo + 1; // 13
    int rowB = rowOR_Topo + 5; // 17

    int rowSaida = rowOR_Topo + 3; // 15

    // ── 2. Roteamento de Fios ─────────────────────────────────────────────
    if (show_A) {
        MH(0, colOR, rowA, vA); // Entra +1 pixel na concavidade
    }
    
    if (show_B) {
        MH(0, colOR, rowB, vB); // Entra +1 pixel na concavidade
    }

    // ── 3. Renderização da Porta Lógica ───────────────────────────────────
    mpOR(colOR, rowOR_Topo, 2); 

    // ── 4. Roteamento de Saída e Atuador (LED) ────────────────────────────
    MH(colOR + 8, 50, rowSaida, vOR);

    for (int r = rowSaida - 1; r <= rowSaida + 1; r++) {
        MH(51, 53, r, vOR);
    }

    // ── 5. Despacho Gráfico ───────────────────────────────────────────────
    renderizarComCores();

    // ── 6. Camada de Texto Sobreposta (Z-Buffer) ──────────────────────────
    uint16_t cor_letras = display->color565(255, 255, 255);
    display->setFont(&TomThumb);
    display->setTextSize(1);
    display->setTextWrap(false);
    display->setTextColor(cor_letras);

    // Texto inferior "OR" posicionado sob a porta: X = (64 - (2 * 4)) / 2 = 28
    display->setCursor(28, 26);
    display->print("OR");

    display->setFont(NULL);
}