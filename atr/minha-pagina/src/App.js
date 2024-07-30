import React from 'react';
import './App.css';

function App() {
  return (
    <div className="container mt-5">
      {/* Cabeçalho */}
      <header className="mb-4">
        <nav className="navbar navbar-light bg-light">
          <span className="navbar-brand">Meu Site</span>
        </nav>
      </header>

      {/* Conteúdo Principal */}
      <main>
        <div className="row">
          {/* Primeiro Grid com 2 Grids dentro */}
          <div className="col-md-12 mb-3">
            <div className="row">
              <div className="col-md-6">
                <div className="p-3 border bg-light">Grid 1 - Dentro</div>
              </div>
              <div className="col-md-6">
                <div className="p-3 border bg-light">Grid 2 - Dentro</div>
              </div>
            </div>
          </div>

          {/* Segundo Grid com 2 Grids dentro */}
          <div className="col-md-12 mb-3">
            <div className="row">
              <div className="col-md-6">
                <div className="p-3 border bg-light">Grid 3 - Dentro</div>
              </div>
              <div className="col-md-6">
                <div className="p-3 border bg-light">Grid 4 - Dentro</div>
              </div>
            </div>
          </div>

          {/* Terceiro Grid */}
          <div className="col-md-12 mb-3">
            <div className="p-3 border bg-light">Grid 5</div>
          </div>

          {/* Quarto Grid */}
          <div className="col-md-12 mb-3">
            <div className="p-3 border bg-light">Grid 6</div>
          </div>

          {/* Quinto Grid */}
          <div className="col-md-12">
            <div className="p-3 border bg-light">Grid 7</div>
          </div>
        </div>
      </main>

      {/* Rodapé */}
      <footer className="mt-4 py-3 bg-light text-center">
        <p>&copy; 2024 Obrigada </p>
      </footer>
    </div>
  );
}

export default App;
