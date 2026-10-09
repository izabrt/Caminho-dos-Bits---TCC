void desenharFaseTutorialAND() {
    inicializarMatriz();

    // Recupera valores lógicos da engine do jogo (Fase de 2 entradas)
    short int vA   = values[0]; // entrada A (cima)
    short int vB   = values[1]; // entrada B (baixo)
    short int vAND = values[2]; // AND(A, B) -> Saída S
    
    // ── 0. Controle de Seleção (Blink IHM) ────────────────────────────────
    bool show_A = !(selected_input == 0 && !blink_state);
    bool show_B = !(selected_input == 1 && !blink_state);

    // ── 1. Geometria Centralizada (Painel 64x32) ──────────────────────────
    int rowAND_Topo = 13; 
    int colAND      = 28; 
    
    int rowA = rowAND_Topo + 1; // 14
    int rowB = rowAND_Topo + 3; // 16
    
    int rowSaida = rowAND_Topo + 2; // 15

    // ── 2. Roteamento de Fios ─────────────────────────────────────────────
    if (show_A) {
        MH(0, colAND - 1, rowA, vA);
    }
    
    if (show_B) {
        MH(0, colAND - 1, rowB, vB);
    }

    // ── 3. Renderização da Porta Lógica ───────────────────────────────────
    mpAND(colAND, rowAND_Topo, 2); // '2' é a constante COR_PORTA branca

    // ── 4. Roteamento de Saída e Atuador (LED) ────────────────────────────
    MH(colAND + 5, 50, rowSaida, vAND);

    for (int r = rowSaida - 1; r <= rowSaida + 1; r++) {
        MH(51, 53, r, vAND);
    }

    // ── 5. Despacho Gráfico ───────────────────────────────────────────────
    // Renderiza o array bidimensional da matriz primeiro
    renderizarComCores();

    // ── 6. Camada de Texto Sobreposta (Z-Buffer) ──────────────────────────
    // Executado após o envio dos pixels para evitar o espelhamento/apagamento
    uint16_t cor_letras = display->color565(255, 255, 255); // Branco estático para legibilidade
    display->setFont(&TomThumb);
    display->setTextSize(1);
    display->setTextWrap(false);
    display->setTextColor(cor_letras);

    // Texto inferior posicionado sob a porta: X = (64 - (3 * 4)) / 2 = 26 | Y = 19 + BASELINE_OFFSET
    display->setCursor(26, 24);
    display->print("AND");

    display->setFont(NULL); // Restaura o ponteiro de fonte padrão do sistema
}